// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef THIRDPARTY_IMGUI_IMGUIHELPER_H_
#define THIRDPARTY_IMGUI_IMGUIHELPER_H_

#include "ThirdParty/ImGui/imconfig.h"

#ifdef __GNUC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
#include <imgui/imgui.h>
#ifdef __GNUC__
#   pragma GCC diagnostic pop
#endif

#include "Input/Pointer.h"
#include "Memory/Array.h"

namespace rainbow
{
    struct KeyStroke;
}

namespace rainbow::imgui
{
    constexpr ImGuiWindowFlags kDefaultWindowFlags =  //
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_MenuBar;

    void init();
    void new_frame(uint64_t dt);
    void render(ImDrawData*);
    bool set_key_state(const KeyStroke& key, bool down);
    bool set_mouse_state(const ArrayView<Pointer>& pointers);
    bool set_mouse_state(const ArrayView<Pointer>& pointers, bool down);
    bool set_mouse_wheel(const ArrayView<Pointer>& wheels);
    void shutdown();
}  // namespace rainbow::imgui

#endif
