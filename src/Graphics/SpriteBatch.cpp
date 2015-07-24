// Copyright (c) 2010-15 Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "Graphics/SpriteBatch.h"

#include <algorithm>

#include "Graphics/Renderer.h"
#include "Graphics/ShaderDetails.h"

namespace
{
	template<typename T>
	class SetBuffer
	{
	public:
		explicit SetBuffer(T *buffer) : buffer_(buffer) {}
		void operator()(Sprite &sprite);

	private:
		T *buffer_;
	};

	class Update
	{
	public:
		Update() : should_update_(false) {}

		void operator()(Sprite &sprite)
		{
			should_update_ |= sprite.update();
		}

		explicit operator bool() const { return should_update_; }

	private:
		bool should_update_;
	};

	template<>
	void SetBuffer<SpriteVertex>::operator()(Sprite &sprite)
	{
		sprite.set_vertex_array(buffer_);
		buffer_ += 4;
	}

	template<>
	void SetBuffer<Vec2f>::operator()(Sprite &sprite)
	{
		sprite.set_normal_buffer(buffer_);
		buffer_ += 4;
	}
}

SpriteBatch::SpriteBatch(const unsigned int hint) : count_(0), reserved_(0)
{
	resize(hint);
	array_.reconfigure(std::bind(&SpriteBatch::bind_arrays, this));
}

SpriteBatch::SpriteBatch(SpriteBatch&& batch)
    : sprites_(std::move(batch.sprites_)),
      vertices_(std::move(batch.vertices_)),
      normals_(std::move(batch.normals_)), count_(batch.count_),
      vertex_buffer_(std::move(batch.vertex_buffer_)),
      normal_buffer_(std::move(batch.normal_buffer_)),
      array_(std::move(batch.array_)), normal_(std::move(batch.normal_)),
      texture_(std::move(batch.texture_)), reserved_(batch.reserved_) {}

SpriteBatch::~SpriteBatch()
{
	sprites_.release(count_);
	vertices_.release(count_ * 4);
	normals_.release(count_ * 4);
}

void SpriteBatch::set_normal(SharedPtr<TextureAtlas> texture)
{
	if (!normals_)
	{
		normals_.resize(0, reserved_ * 4);
		std::uninitialized_fill_n(normals_.get(), count_ * 4, Vec2f());
		set_buffer(normals_.get());
		array_.reconfigure(std::bind(&SpriteBatch::bind_arrays, this));
	}
	normal_ = std::move(texture);
}

void SpriteBatch::set_texture(SharedPtr<TextureAtlas> texture)
{
	texture_ = std::move(texture);
}

Sprite::Ref SpriteBatch::add(const int x, const int y, const int w, const int h)
{
	auto sprite = create_sprite(w, h);
	sprite->set_texture(texture_->define(Vec2i(x, y), w, h));
	return sprite;
}

void SpriteBatch::bind_textures() const
{
	if (normal_)
		normal_->bind(1);
	texture_->bind();
}

void SpriteBatch::bring_to_front(const Sprite::Ref &s)
{
	R_ASSERT(s.batch_ == this, "Sprite does not belong to this batch");

	rotate(s.i_, s.i_ + 1, count_);
}

Sprite::Ref SpriteBatch::create_sprite(const unsigned int width,
                                       const unsigned int height)
{
	R_ASSERT(count_ <= Renderer::kNumSprites, "Hard-coded limit reached");

	if (count_ == reserved_)
	{
		const unsigned int half = reserved_ / 2;
		resize(reserved_ + ((half == 0) ? 1 : half));
	}
	Sprite &sprite = sprites_[count_];
	new (&sprite) Sprite(width, height, this);
	const unsigned int offset = count_ * 4;
	std::uninitialized_fill_n(vertices_ + offset, 4, SpriteVertex());
	sprite.set_vertex_array(vertices_ + offset);
	if (normals_)
	{
		std::uninitialized_fill_n(normals_ + offset, 4, Vec2f());
		sprite.set_normal_buffer(normals_ + offset);
	}
	return Sprite::Ref(this, count_++);
}

void SpriteBatch::erase(const Sprite::Ref &s)
{
	bring_to_front(s);
	sprites_[--count_].~Sprite();
}

Sprite::Ref SpriteBatch::find_sprite_by_id(const int id) const
{
	for (unsigned int i = 0; i < count_; ++i)
	{
		if (sprites_[i].id() == id)
			return {this, i};
	}
	return {};
}

void SpriteBatch::move(const Vec2f &delta)
{
	if (delta.is_zero())
		return;

	std::for_each(sprites_.get(), sprites_ + count_, [&delta](Sprite &sprite) {
		sprite.move(delta);
	});
}

void SpriteBatch::swap(const Sprite::Ref &a, const Sprite::Ref &b)
{
	if (a == b)
		return;

	std::swap(*a, *b);

	const size_t ia = a.i_ * 4;
	const size_t ib = b.i_ * 4;

	std::swap_ranges(vertices_ + ia, vertices_ + ia + 4, vertices_ + ib);
	if (normals_)
		std::swap_ranges(normals_ + ia, normals_ + ia + 4, normals_ + ib);
}

void SpriteBatch::update()
{
	// Update all sprites, then upload the vertex buffer if necessary.
	if (std::for_each(sprites_.get(), sprites_ + count_, Update()))
	{
		const unsigned int count = count_ * 4;
		vertex_buffer_.upload(vertices_.get(), count * sizeof(SpriteVertex));
		if (normals_)
			normal_buffer_.upload(normals_.get(), count * sizeof(Vec2f));
	}
}

void SpriteBatch::bind_arrays() const
{
	vertex_buffer_.bind();
	if (normals_)
		normal_buffer_.bind(Shader::kAttributeNormal);
}

void SpriteBatch::resize(const unsigned int size)
{
	sprites_.resize(count_, size);
	vertices_.resize(count_ * 4, size * 4);
	set_buffer(vertices_.get());
	if (normals_)
	{
		normals_.resize(count_ * 4, size * 4);
		set_buffer(normals_.get());
	}
	reserved_ = size;
}

void SpriteBatch::rotate(size_t first, size_t n_first, size_t last)
{
	std::rotate(sprites_ + first, sprites_ + n_first, sprites_ + last);

	first *= 4;
	n_first *= 4;
	last *= 4;

	std::rotate(vertices_ + first, vertices_ + n_first, vertices_ + last);
	if (normals_)
		std::rotate(normals_ + first, normals_ + n_first, normals_ + last);
}

template<typename T>
void SpriteBatch::set_buffer(T *buffer)
{
	std::for_each(sprites_.get(), sprites_ + count_, SetBuffer<T>(buffer));
}
