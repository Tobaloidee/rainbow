// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef GRAPHICS_RENDERQUEUE_H_
#define GRAPHICS_RENDERQUEUE_H_

#include <memory>
#include <string>
#include <vector>

#include "Common/String.h"
#include "Common/Variant.h"

namespace rainbow
{
    class Animation;
    class IDrawable;
    class Label;
    class SpriteBatch;
}  // namespace rainbow

namespace rainbow::graphics
{
    class RenderUnit
    {
    public:
        using variant_type = variant<  //
            Animation*,
            IDrawable*,
            Label*,
            SpriteBatch*>;

        template <typename T>
        RenderUnit(T& variant, std::string tag = {})
            : enabled_(true), variant_(&variant), tag_(std::move(tag))
        {
        }

        template <typename T>
        RenderUnit(std::shared_ptr<T>& variant, std::string tag = {})
            : RenderUnit(*variant, std::move(tag))
        {
        }

        template <typename T>
        RenderUnit(std::unique_ptr<T>& variant, std::string tag = {})
            : RenderUnit(*variant, std::move(tag))
        {
        }

        auto is_enabled() const { return enabled_; }
        auto object() const -> const variant_type& { return variant_; }
        auto tag() const -> const std::string& { return tag_; }

        void set_tag(std::string tag) { tag_ = std::move(tag); }

        void disable() { enabled_ = false; }
        void enable() { enabled_ = true; }

        friend bool operator==(const RenderUnit& lhs, const RenderUnit& rhs)
        {
            return lhs.variant_ == rhs.variant_;
        }

        friend bool operator!=(const RenderUnit& lhs, const RenderUnit& rhs)
        {
            return !(lhs == rhs);
        }

        friend bool operator==(czstring tag, const RenderUnit& unit)
        {
            return tag == unit.tag();
        }

        friend bool operator==(const RenderUnit& unit, czstring tag)
        {
            return unit.tag() == tag;
        }

    private:
        bool enabled_;
        variant_type variant_;
        std::string tag_;
    };

    using RenderQueue = std::vector<RenderUnit>;

    void draw(RenderQueue& queue);

    void update(RenderQueue& queue, uint64_t dt);
}  // namespace rainbow::graphics

#endif
