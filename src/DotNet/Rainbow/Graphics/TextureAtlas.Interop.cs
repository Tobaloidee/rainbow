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
    /// Contains unmanaged delegates for <see cref="TextureAtlas"/>.
    /// </content>
    public sealed partial class TextureAtlas : NativeObject<TextureAtlas>
    {
        private static FuncCFP ctor;
        private static MemberFuncI getHeight;
        private static MemberFuncI getWidth;
        private static MemberFuncB isValid;
        private static MemberFuncIIIIU addRegion;
        private static MemberFuncUU getTextureRegion;
        private static MemberActionIpI setRegions;

        internal static void InitializeComponents(IntPtr ptr)
        {
            var ftable = InitializeComponents<UTextureAtlas>(ptr);
            GetDelegateForFunctionPointer(ftable.CtorFunc, out ctor);
            GetDelegateForFunctionPointer(ftable.GetHeightFunc, out getHeight);
            GetDelegateForFunctionPointer(ftable.GetWidthFunc, out getWidth);
            GetDelegateForFunctionPointer(ftable.IsValidFunc, out isValid);
            GetDelegateForFunctionPointer(ftable.AddRegionFunc, out addRegion);
            GetDelegateForFunctionPointer(ftable.GetTextureRegionFunc, out getTextureRegion);
            GetDelegateForFunctionPointer(ftable.SetRegionsFunc, out setRegions);
        }
    }
}
