// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "Input/Input.h"

#define kInvalidControllerAxis    "Invalid controller axis"
#define kInvalidControllerButton  "Invalid controller button"
#define kInvalidVirtualKey        "Invalid virtual key"

using rainbow::ControllerAxisMotion;
using rainbow::ControllerButtonEvent;
using rainbow::Input;
using rainbow::InputListener;
using rainbow::KeyStroke;
using rainbow::Passkey;
using rainbow::Pointer;
using rainbow::VirtualKey;

constexpr unsigned int Input::kNumSupportedControllers;  // NOLINT(readability-redundant-declaration)

template <typename F>
void Input::process_controller(unsigned int id, F&& process)
{
    for (auto i = 0u; i < controllers_.size(); ++i)
    {
        if (controllers_[i].id() == id)
        {
            process(i);
            break;
        }
    }
}

void Input::subscribe(InputListener& listener)
{
    last_listener_->append(listener);
    last_listener_ = &listener;
}

void Input::unsubscribe(InputListener& listener)
{
    listener.pop();
}

void Input::accelerated(double x, double y, double z, double t)
{
    acceleration_.update(x, y, z, t);
}

void Input::on_controller_axis_motion(const ControllerAxisMotion& motion)
{
    R_ASSERT(to_underlying_type(motion.axis) >= 0 &&
                 motion.axis < ControllerAxis::Count,
             kInvalidControllerAxis);

    process_controller(motion.id, [this, motion](unsigned int i) {
        controllers_[i].on_axis_motion(motion);
        auto event = motion;
        event.id = i;
        for_each(next(), [&event](InputListener& listener) {
            return listener.on_controller_axis_motion(event);
        });
    });
}

void Input::on_controller_button_down(const ControllerButtonEvent& button)
{
    R_ASSERT(to_underlying_type(button.button) >= 0 &&
                 button.button < ControllerButton::Count,
             kInvalidControllerButton);

    process_controller(button.id, [this, button](unsigned int i) {
        controllers_[i].on_button_down(button);
        auto event = button;
        event.id = i;
        for_each(next(), [&event](InputListener& listener) {
            return listener.on_controller_button_down(event);
        });
    });
}

void Input::on_controller_button_up(const ControllerButtonEvent& button)
{
    R_ASSERT(to_underlying_type(button.button) >= 0 &&
                 button.button < ControllerButton::Count,
             kInvalidControllerButton);

    process_controller(button.id, [this, button](unsigned int i) {
        controllers_[i].on_button_up(button);
        auto event = button;
        event.id = i;
        for_each(next(), [&event](InputListener& listener) {
            return listener.on_controller_button_up(event);
        });
    });
}

void Input::on_controller_connected(unsigned int id)
{
    int port = -1;
    for (auto i = 0u; i < controllers_.size(); ++i)
    {
        if (controllers_[i].id() == id)
            return;

        if (port < 0 && !controllers_[i].is_assigned())
            port = narrow_cast<int>(i);
    }

    if (port < 0)
        return;

    controllers_[port].assign(id);
    LOGI("Controller %u plugged into port %d", id, port + 1);

    for_each(next(), [port](InputListener& listener) {
        return listener.on_controller_connected(port);
    });
}

void Input::on_controller_disconnected(unsigned int id)
{
    process_controller(id, [this](unsigned int i) {
        auto& controller = controllers_[i];
        LOGI("Controller %u unplugged from port %d", controller.id(), i + 1);
        controller.unassign();

        for_each(next(), [i](InputListener& listener) {
            return listener.on_controller_disconnected(i);
        });
    });
}

void Input::on_key_down(const KeyStroke& k)
{
    R_ASSERT(to_underlying_type(k.key) >= 0 && k.key < VirtualKey::KeyCount,
             kInvalidVirtualKey);

    keys_[to_underlying_type(k.key)] = true;
    for_each(next(), [&k](InputListener& listener) {  //
        return listener.on_key_down(k);
    });
}

void Input::on_key_up(const KeyStroke& k)
{
    R_ASSERT(to_underlying_type(k.key) >= 0 && k.key < VirtualKey::KeyCount,
             kInvalidVirtualKey);

    keys_[to_underlying_type(k.key)] = false;
    for_each(next(), [&k](InputListener& listener) {  //
        return listener.on_key_up(k);
    });
}

void Input::on_last_listener_changed(InputListener& new_listener,
                                     Passkey<InputListener>)
{
    last_listener_ = &new_listener;
}

void Input::on_mouse_wheel(const ArrayView<Pointer>& wheels)
{
    for_each(next(), [&wheels](InputListener& listener) {
        return listener.on_mouse_wheel(wheels);
    });
}

void Input::on_pointer_began(const ArrayView<Pointer>& pointers)
{
    for_each(next(), [&pointers](InputListener& listener) {
        return listener.on_pointer_began(pointers);
    });
}

void Input::on_pointer_canceled()
{
    for_each(next(), [](InputListener& listener) {
        return listener.on_pointer_canceled();
    });
}

void Input::on_pointer_ended(const ArrayView<Pointer>& pointers)
{
    for_each(next(), [&pointers](InputListener& listener) {
        return listener.on_pointer_ended(pointers);
    });
}

void Input::on_pointer_moved(const ArrayView<Pointer>& pointers)
{
    for_each(next(), [&pointers](InputListener& listener) {
        return listener.on_pointer_moved(pointers);
    });
}

void InputListener::on_end_link_changed(InputListener& new_link)
{
    auto input = Input::Get();
    if (input == nullptr)
        return;

    input->on_last_listener_changed(new_link, {});
}
