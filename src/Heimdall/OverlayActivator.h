// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef HEIMDALL_OVERLAYACTIVATOR_H_
#define HEIMDALL_OVERLAYACTIVATOR_H_

#include <cstdint>

#include "Input/InputListener.h"

namespace heimdall
{
    class Overlay;

    class OverlayActivator final : public rainbow::InputListener
    {
    public:
        OverlayActivator(Overlay* overlay) : overlay_(overlay) { reset(); }

        bool is_activated() const
        {
            return time_till_activation_ == 0 && resistance_ < 2;
        }

        void reset();
        void update(uint64_t dt);

    private:
        int resistance_;
        int time_till_activation_;
        uint32_t pointers_[2];
        Overlay* overlay_;

        // InputListener implementation details

        bool on_key_down_impl(const rainbow::KeyStroke&) override;
        bool on_pointer_began_impl(const ArrayView<rainbow::Pointer>&) override;
        bool on_pointer_canceled_impl() override;
        bool on_pointer_ended_impl(const ArrayView<rainbow::Pointer>&) override;
    };
}  // namespace heimdall

#endif  // HEIMDALL_OVERLAYACTIVATOR_H_
