// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "Graphics/SpriteBatch.h"

#include <gtest/gtest.h>

#include "Tests/TestHelpers.h"

using rainbow::Sprite;
using rainbow::SpriteBatch;
using rainbow::SpriteRef;
using rainbow::SpriteVertex;
using rainbow::TextureAtlas;
using rainbow::Vec2f;

namespace
{
    template <int N>
    void set_sprite_ids(SpriteRef (&refs)[N])
    {
        for (int i = 0; i < N; ++i)
            refs[i]->set_id(i + 1);

        for (int i = 0; i < N; ++i)
            ASSERT_EQ(refs[i]->id(), i + 1);
    }

    void update(SpriteBatch& batch)
    {
        auto sprites = batch.sprites();
        for (uint32_t i = 0; i < batch.size(); ++i)
        {
            ArraySpan<SpriteVertex> buffer{batch.vertices() + i * 4, 4};
            sprites[i].update(buffer, batch.texture());
        }
    }

    void verify_sprite_vertices(const Sprite& sprite,
                                const SpriteVertex* vertices,
                                const Vec2f& offset)
    {
        const float half_width = sprite.width() * 0.5f;
        const float half_height = sprite.height() * 0.5f;
        ASSERT_EQ(vertices[0].position,
                  Vec2f(-half_width + offset.x, -half_height + offset.y));
        ASSERT_EQ(vertices[1].position,
                  Vec2f(half_width + offset.x, -half_height + offset.y));
        ASSERT_EQ(vertices[2].position,
                  Vec2f(half_width + offset.x, half_height + offset.y));
        ASSERT_EQ(vertices[3].position,
                  Vec2f(-half_width + offset.x, half_height + offset.y));
    }

    void verify_sprites_vertices(const SpriteBatch& batch)
    {
        for (auto&& sprite : batch)
            verify_sprite_vertices(sprite, sprite.vertex_array(), Vec2f::Zero);
    }

    void verify_batch_integrity(const SpriteBatch& batch)
    {
        char trace[16];
        const auto sprites = batch.sprites();
        const SpriteVertex* vertices = batch.vertices();
        for (uint32_t i = 0; i < batch.size(); ++i)
        {
            snprintf(trace, sizeof(trace), "i = %u", i);
            SCOPED_TRACE(trace);
            auto vertex_array = vertices + i * 4;
            ASSERT_EQ(sprites[i].vertex_array(), vertex_array);
            verify_sprite_vertices(sprites[i], vertex_array, Vec2f::Zero);
        }
    }

    class SpriteBatchOperationsTest : public ::testing::Test
    {
    public:
        SpriteBatchOperationsTest()
            : batch(rainbow::ISolemnlySwearThatIAmOnlyTesting{}),
              refs{batch.create_sprite(1, 1),
                   batch.create_sprite(2, 1),
                   batch.create_sprite(1, 2),
                   batch.create_sprite(2, 2)},
              count(rainbow::array_size(refs)), vertices(batch.vertices())
        {
        }

    protected:
        SpriteBatch batch;
        SpriteRef refs[4];
        const size_t count;
        const SpriteVertex* vertices;

        void SetUp() override { update(batch); }
    };
}  // namespace

TEST(SpriteBatchTest, MoveConstructs)
{
    rainbow::ISolemnlySwearThatIAmOnlyTesting mock;

    SpriteBatch batch(mock);
    auto atlas = rainbow::make_shared<TextureAtlas>(mock);
    batch.set_texture(atlas);

    for (uint32_t i = 0; i < batch.capacity(); ++i)
        batch.create_sprite(i + 1, i + 1);
    update(batch);

    const uint32_t vertex_count = batch.vertex_count();
    const Sprite* sprites_array = batch.sprites();
    const TextureAtlas& texture = batch.texture();
    const uint32_t sprite_count = batch.size();
    const SpriteVertex* vertices = batch.vertices();

    SpriteBatch moved(std::move(batch));

    ASSERT_EQ(moved.vertex_count(), vertex_count);
    ASSERT_EQ(moved.sprites(), sprites_array);
    ASSERT_EQ(&moved.texture(), &texture);
    ASSERT_EQ(moved.size(), sprite_count);
    ASSERT_EQ(moved.vertices(), vertices);

    for (uint32_t i = 0; i < moved.size(); ++i)
    {
        const float s = (i + 1) * 0.5f;
        const uint32_t j = i * 4;
        ASSERT_EQ(vertices[j].position, Vec2f(-s, -s));
        ASSERT_EQ(vertices[j + 1].position, Vec2f(s, -s));
        ASSERT_EQ(vertices[j + 2].position, Vec2f(s, s));
        ASSERT_EQ(vertices[j + 3].position, Vec2f(-s, s));
    }

    ASSERT_EQ(batch.sprites(), nullptr);
    ASSERT_EQ(batch.vertices(), nullptr);
    ASSERT_EQ(batch.vertex_count(), 0u);
}

TEST(SpriteBatchTest, CreatesSprites)
{
    SpriteBatch batch(rainbow::ISolemnlySwearThatIAmOnlyTesting{});
    SpriteRef sprites[]{
        batch.create_sprite(1, 1),
        batch.create_sprite(2, 2),
        batch.create_sprite(3, 3),
        batch.create_sprite(4, 4),
    };
    set_sprite_ids(sprites);
    update(batch);

    ASSERT_EQ(sprites[0]->width(), 1u);
    ASSERT_EQ(sprites[1]->width(), 2u);
    ASSERT_EQ(sprites[2]->width(), 3u);
    ASSERT_EQ(sprites[3]->width(), 4u);

    verify_batch_integrity(batch);

    batch.erase(sprites[2]);

    ASSERT_EQ(sprites[0]->width(), 1u);
    ASSERT_EQ(sprites[1]->width(), 2u);
    ASSERT_EQ(sprites[3]->width(), 4u);

    sprites[2] = batch.create_sprite(7, 7);

    ASSERT_EQ(sprites[0]->width(), 1u);
    ASSERT_EQ(sprites[1]->width(), 2u);
    ASSERT_EQ(sprites[2]->width(), 7u);
    ASSERT_EQ(sprites[3]->width(), 4u);

    update(batch);

    verify_batch_integrity(batch);
}

TEST(SpriteBatchTest, IsVisible)
{
    SpriteBatch batch(rainbow::ISolemnlySwearThatIAmOnlyTesting{});
    batch.create_sprite(1, 1);

    ASSERT_TRUE(batch.is_visible());
    ASSERT_EQ(batch.vertex_count(), 6u);

    batch.set_visible(true);

    ASSERT_TRUE(batch.is_visible());
    ASSERT_EQ(batch.vertex_count(), 6u);

    batch.set_visible(false);

    ASSERT_FALSE(batch.is_visible());
    ASSERT_EQ(batch.vertex_count(), 0u);

    batch.set_visible(false);

    ASSERT_FALSE(batch.is_visible());
    ASSERT_EQ(batch.vertex_count(), 0u);

    batch.set_visible(true);

    ASSERT_TRUE(batch.is_visible());
    ASSERT_EQ(batch.vertex_count(), 6u);

    SpriteBatch batch2(std::move(batch));

    ASSERT_TRUE(batch2.is_visible());
    ASSERT_EQ(batch2.vertex_count(), 6u);

    batch2.set_visible(false);
    SpriteBatch batch3(std::move(batch2));

    ASSERT_FALSE(batch3.is_visible());
    ASSERT_EQ(batch3.vertex_count(), 0u);
}

TEST(SpriteBatchTest, ExcessSpritesAreDiscarded)
{
    SpriteBatch batch(rainbow::ISolemnlySwearThatIAmOnlyTesting{});
    uint32_t capacity = batch.capacity();
    for (uint32_t i = 0; i < capacity; ++i)
        batch.create_sprite(i + 1, i + 1);
    update(batch);

    ASSERT_EQ(batch.size(), capacity);

    auto&& vertices = batch.vertices();
    for (uint32_t i = 0; i < batch.size(); ++i)
    {
        const float s = (i + 1) * 0.5f;
        const uint32_t j = i * 4;
        ASSERT_EQ(vertices[j].position, Vec2f(-s, -s));
        ASSERT_EQ(vertices[j + 1].position, Vec2f(s, -s));
        ASSERT_EQ(vertices[j + 2].position, Vec2f(s, s));
        ASSERT_EQ(vertices[j + 3].position, Vec2f(-s, s));
    }

    batch.create_sprite(capacity + 1, capacity + 1);

    ASSERT_EQ(batch.size(), capacity);

    for (uint32_t i = 0; i < batch.size(); ++i)
    {
        const float s = (i + 1) * 0.5f;
        const uint32_t j = i * 4;
        ASSERT_EQ(vertices[j].position, Vec2f(-s, -s));
        ASSERT_EQ(vertices[j + 1].position, Vec2f(s, -s));
        ASSERT_EQ(vertices[j + 2].position, Vec2f(s, s));
        ASSERT_EQ(vertices[j + 3].position, Vec2f(-s, s));
    }

    batch.create_sprite(capacity + 1, capacity + 1);
    batch.create_sprite(capacity + 1, capacity + 1);
    batch.create_sprite(capacity + 1, capacity + 1);

    ASSERT_EQ(batch.size(), capacity);

    for (uint32_t i = 0; i < batch.size(); ++i)
    {
        const float s = (i + 1) * 0.5f;
        const uint32_t j = i * 4;
        ASSERT_EQ(vertices[j].position, Vec2f(-s, -s));
        ASSERT_EQ(vertices[j + 1].position, Vec2f(s, -s));
        ASSERT_EQ(vertices[j + 2].position, Vec2f(s, s));
        ASSERT_EQ(vertices[j + 3].position, Vec2f(-s, s));
    }
}

TEST_F(SpriteBatchOperationsTest, SpritesShareASingleBuffer)
{
    ASSERT_EQ(batch.vertex_count(), count * 6);

    const auto sprites = batch.sprites();
    for (size_t i = 0; i < count; ++i)
    {
        ASSERT_EQ(&(*refs[i]), &sprites[i]);
        ASSERT_EQ(refs[i]->vertex_array(), vertices + i * 4);
    }
}

TEST_F(SpriteBatchOperationsTest, SpritesPositionAtOriginOnCreation)
{
    update(batch);
    for (size_t i = 0; i < count; ++i)
    {
        ASSERT_EQ(refs[i]->position(), Vec2f::Zero);
        verify_sprite_vertices(*refs[i], vertices + i * 4, Vec2f::Zero);
    }
}

TEST_F(SpriteBatchOperationsTest, BringsSpritesToFront)
{
    set_sprite_ids(refs);
    update(batch);

    batch.bring_to_front(refs[1]);

    ASSERT_EQ(batch[0].id(), 1);
    ASSERT_EQ(batch[1].id(), 2);
    ASSERT_EQ(batch[2].id(), 3);
    ASSERT_EQ(batch[3].id(), 4);

    auto sprites = batch.sprites();

    ASSERT_EQ(sprites[0].id(), 1);
    ASSERT_EQ(sprites[1].id(), 3);
    ASSERT_EQ(sprites[2].id(), 4);
    ASSERT_EQ(sprites[3].id(), 2);

    verify_sprites_vertices(batch);

    update(batch);

    verify_batch_integrity(batch);
}

TEST_F(SpriteBatchOperationsTest, ClearsSprites)
{
    const auto capacity = batch.capacity();

    ASSERT_EQ(batch.size(), count);

    batch.clear();

    ASSERT_EQ(batch.vertex_count(), 0u);
    ASSERT_EQ(batch.size(), 0u);
    ASSERT_EQ(batch.capacity(), capacity);
}

TEST_F(SpriteBatchOperationsTest, ErasesSprites)
{
    set_sprite_ids(refs);
    update(batch);

    batch.erase(batch.find_sprite_by_id(2));

    size_t length = count - 1;
    ASSERT_EQ(batch.vertex_count(), length * 6);

    auto sprites = batch.sprites();
    ASSERT_EQ(sprites[0].id(), 1);
    ASSERT_EQ(sprites[1].id(), 3);
    ASSERT_EQ(sprites[2].id(), 4);

    verify_sprites_vertices(batch);

    update(batch);

    verify_batch_integrity(batch);

    batch.erase(batch.find_sprite_by_id(1));

    ASSERT_EQ(batch.vertex_count(), --length * 6);
    ASSERT_EQ(sprites[0].id(), 3);
    ASSERT_EQ(sprites[1].id(), 4);

    verify_sprites_vertices(batch);

    update(batch);

    verify_batch_integrity(batch);

    batch.erase(batch.find_sprite_by_id(4));
    update(batch);

    ASSERT_EQ(batch.vertex_count(), 6u);
    ASSERT_EQ(sprites[0].id(), 3);
    ASSERT_EQ(sprites[0].vertex_array(), vertices);
    verify_sprite_vertices(sprites[0], vertices, Vec2f::Zero);

    batch.erase(batch.find_sprite_by_id(3));

    ASSERT_EQ(batch.vertex_count(), 0u);
}

TEST_F(SpriteBatchOperationsTest, FindsSpritesById)
{
    set_sprite_ids(refs);

    for (auto&& sprite : refs)
        ASSERT_EQ(&(*sprite), &(*batch.find_sprite_by_id(sprite->id())));

    ASSERT_FALSE(batch.find_sprite_by_id(0xdeadbeef));
}

TEST_F(SpriteBatchOperationsTest, MovesSprites)
{
    batch.move(Vec2f::One);
    update(batch);
    for (size_t i = 0; i < count; ++i)
        verify_sprite_vertices(*refs[i], vertices + i * 4, Vec2f::One);

    batch.move(-Vec2f::One);
    update(batch);

    verify_batch_integrity(batch);
}

TEST_F(SpriteBatchOperationsTest, SwapsSprites)
{
    set_sprite_ids(refs);
    update(batch);

    batch.swap(refs[0], refs[0]);

    auto sprites = batch.sprites();
    ASSERT_EQ(sprites[0].id(), 1);
    ASSERT_EQ(sprites[1].id(), 2);
    ASSERT_EQ(sprites[2].id(), 3);
    ASSERT_EQ(sprites[3].id(), 4);

    verify_sprites_vertices(batch);

    update(batch);

    verify_batch_integrity(batch);

    batch.swap(refs[0], refs[3]);

    ASSERT_EQ(sprites[0].id(), 4);
    ASSERT_EQ(sprites[1].id(), 2);
    ASSERT_EQ(sprites[2].id(), 3);
    ASSERT_EQ(sprites[3].id(), 1);

    verify_sprites_vertices(batch);

    update(batch);

    verify_batch_integrity(batch);

    batch.swap(refs[1], refs[2]);

    ASSERT_EQ(sprites[0].id(), 4);
    ASSERT_EQ(sprites[1].id(), 3);
    ASSERT_EQ(sprites[2].id(), 2);
    ASSERT_EQ(sprites[3].id(), 1);

    verify_sprites_vertices(batch);

    update(batch);

    verify_batch_integrity(batch);
}
