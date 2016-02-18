// This file was generated by Rainbow for .NET Core. Do not modify.
//
// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

namespace Rainbow.Graphics
{
    using System;

    using Interop;

    /// <content>
    /// Contains unmanaged delegates for <see cref="RenderQueue"/>.
    /// </content>
    public sealed partial class RenderQueue : NativeObject<RenderQueue>
    {
        private static MemberFuncPCI addAnimation;
        private static MemberFuncPCI addLabel;
        private static MemberFuncPCI addSpritebatch;
        private static MemberAction clear;
        private static MemberActionI disable;
        private static MemberActionC disableTag;
        private static MemberActionI enable;
        private static MemberActionC enableTag;
        private static MemberFuncIPCI insertAnimation;
        private static MemberFuncIPCI insertLabel;
        private static MemberFuncIPCI insertSpritebatch;
        private static MemberActionI removeAt;
        private static MemberActionC remove;
        private static MemberActionIC setTag;

        internal static void InitializeComponents(IntPtr ptr)
        {
            var ftable = InitializeComponents<URenderQueue>(ptr);
            GetDelegateForFunctionPointer(ftable.AddAnimationFunc, out addAnimation);
            GetDelegateForFunctionPointer(ftable.AddLabelFunc, out addLabel);
            GetDelegateForFunctionPointer(ftable.AddSpritebatchFunc, out addSpritebatch);
            GetDelegateForFunctionPointer(ftable.ClearFunc, out clear);
            GetDelegateForFunctionPointer(ftable.DisableFunc, out disable);
            GetDelegateForFunctionPointer(ftable.DisableTagFunc, out disableTag);
            GetDelegateForFunctionPointer(ftable.EnableFunc, out enable);
            GetDelegateForFunctionPointer(ftable.EnableTagFunc, out enableTag);
            GetDelegateForFunctionPointer(ftable.InsertAnimationFunc, out insertAnimation);
            GetDelegateForFunctionPointer(ftable.InsertLabelFunc, out insertLabel);
            GetDelegateForFunctionPointer(ftable.InsertSpritebatchFunc, out insertSpritebatch);
            GetDelegateForFunctionPointer(ftable.RemoveAtFunc, out removeAt);
            GetDelegateForFunctionPointer(ftable.RemoveFunc, out remove);
            GetDelegateForFunctionPointer(ftable.SetTagFunc, out setTag);
        }
    }
}
