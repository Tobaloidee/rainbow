// This file was generated by Rainbow for .NET Core. Do not modify.
//
// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

namespace Rainbow.Interop
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    internal sealed class ULabel : ITable
    {
        private readonly uint sizeOfStruct;
        private readonly IntPtr ctorFunc;
        private readonly IntPtr dtorFunc;
        private readonly IntPtr getAlignmentFunc;
        private readonly IntPtr getAngleFunc;
        private readonly IntPtr getColorFunc;
        private readonly IntPtr getPositionFunc;
        private readonly IntPtr getScaleFunc;
        private readonly IntPtr getWidthFunc;
        private readonly IntPtr setColorFunc;
        private readonly IntPtr setFontFunc;
        private readonly IntPtr setAlignmentFunc;
        private readonly IntPtr setPositionFunc;
        private readonly IntPtr setRotationFunc;
        private readonly IntPtr setScaleFunc;
        private readonly IntPtr setTextFunc;

        public uint SizeOfStruct => sizeOfStruct;

        public IntPtr CtorFunc => ctorFunc;

        public IntPtr DtorFunc => dtorFunc;

        public IntPtr GetAlignmentFunc => getAlignmentFunc;

        public IntPtr GetAngleFunc => getAngleFunc;

        public IntPtr GetColorFunc => getColorFunc;

        public IntPtr GetPositionFunc => getPositionFunc;

        public IntPtr GetScaleFunc => getScaleFunc;

        public IntPtr GetWidthFunc => getWidthFunc;

        public IntPtr SetColorFunc => setColorFunc;

        public IntPtr SetFontFunc => setFontFunc;

        public IntPtr SetAlignmentFunc => setAlignmentFunc;

        public IntPtr SetPositionFunc => setPositionFunc;

        public IntPtr SetRotationFunc => setRotationFunc;

        public IntPtr SetScaleFunc => setScaleFunc;

        public IntPtr SetTextFunc => setTextFunc;
    }
}
