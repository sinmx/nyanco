#pragma once

/*!
    @file   KeyCode.h
    @author dasyprocta
 */

#include "base.h"

BEGIN_NAMESPACE_NYANCO

// ============================================================================
struct KeyCode
{
    enum Type
    {
        Unknown = -1,

        // 連番を保証
        Num0 = 0x30,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        // 連番を保証
        CharA = 0x41,
        CharB,
        CharC,
        CharD,
        CharE,
        CharF,
        CharG,
        CharH,
        CharI,
        CharJ,
        CharK,
        CharL,
        CharM,
        CharN,
        CharO,
        CharP,
        CharQ,
        CharR,
        CharS,
        CharT,
        CharU,
        CharV,
        CharW,
        CharX,
        CharY,
        CharZ,

        // 連番を保証
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        Left,
        Up,
        Right,
        Down,

        Escape,
        Space,
        BackSpace,
        Delete,
        Tab,

        BracketL,
        BracketR,
        Return,

        ControlL,
        SemiColon,
        Apostrophe,

        BackSlash,
        ShiftL,
        ShiftR,

        Comma,

        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        NumpadEnter,

        MouseButtonLeft,
        MouseButtonRight,
        MouseButtonMiddle,
    };
};

END_NAMESPACE_NYANCO
