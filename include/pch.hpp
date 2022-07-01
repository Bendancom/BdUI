#ifndef BDUI_PCH
#define BDUI_PCH

#include <string>
#include <typeinfo>
#include <future>
#include <thread>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <WinUser.h>
#include <WinBase.h>
#include <windowsx.h>
#include <wingdi.h>
#endif

#include "OpenGL/glad/glad.h"

#include "attribute.hpp"
#include "delegate.hpp"
#include "event.hpp"
#include "readonly.hpp"
#include "resource/resource.hpp"
#include "resource/cursor.hpp"
#include "resource/bitmap.hpp"
#include "resource/icon.hpp"

namespace BdUI
{
    class UI;
    struct Point{
        long X = 0;
        long Y = 0;
    };
    struct Size{
        unsigned long Width = 0;
        unsigned long Height = 0;
    };
    struct Margin{
        int Left = 0;
        int Right = 0;
        int Top = 0;
        int Bottom = 0;
    };
    struct Mouse{
        Point Location;
        struct {
            unsigned Left : 1;
            unsigned Right : 1;
            unsigned Middle : 1;
            unsigned X1 : 1;
            unsigned X2 : 1;
            unsigned Ctrl : 1;
            unsigned Shift : 1;
            unsigned Alt : 1;
        }Button{0,0,0,0,0};    //Up = 0, Down = 1;
        struct{
            unsigned IsLeaved : 1;  // false = 0, true = 1
            unsigned Hover_Move : 1;  //Hover = 0, Move = 1
            unsigned Client_Border : 1; //Client =0, Border = 1
        }Content{0,0,0};
        int WheelDelta = 0;
    };

    struct Key {
        int RepeatCount;
        char Code;
        unsigned char ScanCode;
        bool Up_Down; // Up = 0,Down = 1
        enum Type {
#ifdef WIN32
            //0x7 undefined
            BackSpace = VK_BACK,
            Tab = VK_TAB,
            //0xA - 0xB Rreserved
            Clear = VK_CLEAR,
            Enter = VK_RETURN,
            //0xE - 0xF undefined
            Control = VK_CONTROL,
            Shift = VK_SHIFT,
            Alt = VK_MENU,
            Pause = VK_PAUSE,
            CapsLock = VK_CAPITAL,
            //0x15 - 0x1A IME
            Esc = VK_ESCAPE,
            Space = VK_SPACE,
            PageUp = VK_PRIOR,
            PageDown = VK_NEXT,
            End = VK_END,
            Home = VK_HOME,
            Left = VK_LEFT,
            Right = VK_RIGHT,
            Up = VK_UP,
            Down = VK_DOWN,
            Select = VK_SELECT,
            Print = VK_PRINT,
            Execute = VK_EXECUTE,
            PrtScrn = VK_SNAPSHOT,
            Insert = VK_INSERT,
            Delete = VK_DELETE,
            Help = VK_HELP,
            Key_0 = 0x30,
            Key_1 = 0x31,
            Key_2 = 0x32,
            Key_3 = 0x33,
            Key_4 = 0x34,
            Key_5 = 0x35,
            Key_6 = 0x36,
            Key_7 = 0x37,
            Key_8 = 0x38,
            Key_9 = 0x39,
            //0x3A - 0x40 Undefined
            A = 0x41,
            B = 0x41,
            C = 0x41,
            D = 0x41,
            E = 0x41,
            F = 0x41,
            G = 0x41,
            H = 0x41,
            I = 0x41,
            J = 0x41,
            K = 0x41,
            L = 0x41,
            M = 0x41,
            N = 0x41,
            O = 0x41,
            P = 0x41,
            Q = 0x41,
            R = 0x41,
            S = 0x41,
            T = 0x41,
            U = 0x41,
            V = 0x41,
            W = 0x41,
            X = 0x41,
            Y = 0x41,
            Z = 0x41,
            LWin = VK_LWIN,
            RWin = VK_RWIN,
            Apps = VK_APPS,
            //0x5E - Reserved
            Sleep = VK_SLEEP,
            Num0 = VK_NUMPAD0,
            Num1 = VK_NUMPAD1,
            Num2 = VK_NUMPAD2,
            Num3 = VK_NUMPAD3,
            Num4 = VK_NUMPAD4,
            Num5 = VK_NUMPAD5,
            Num6 = VK_NUMPAD6,
            Num7 = VK_NUMPAD7,
            Num8 = VK_NUMPAD8,
            Num9 = VK_NUMPAD9,
            Multiply = VK_MULTIPLY,
            Add = VK_ADD,
            Separator = VK_SEPARATOR,
            Subtract = VK_SUBTRACT,
            Decimal = VK_DECIMAL,
            Divide = VK_DIVIDE,
            F1 = VK_F1,
            F2 = VK_F2,
            F3 = VK_F3,
            F4 = VK_F4,
            F5 = VK_F5,
            F6 = VK_F6,
            F7 = VK_F7,
            F8 = VK_F8,
            F9 = VK_F9,
            F10 = VK_F10,
            F11 = VK_F11,
            F12 = VK_F12,
            F13 = VK_F13,
            F14 = VK_F14,
            F15 = VK_F15,
            F16 = VK_F16,
            F17 = VK_F17,
            F18 = VK_F18,
            F19 = VK_F19,
            F20 = VK_F20,
            F21 = VK_F21,
            F22 = VK_F22,
            F23 = VK_F23,
            F24 = VK_F24,
            //0x88 - 0x8F Unassigned
            NumLock = VK_NUMLOCK,
            ScrollLock = VK_SCROLL,
            //0x92 - 0x96 OEM specific
            //0x97 - 0x9F Unassigned
            LShift = VK_LSHIFT,
            RShift = VK_RSHIFT,
            LControl = VK_LCONTROL,
            RControl = VK_RCONTROL,
            LMenu = VK_LMENU,
            RMenu = VK_RMENU,
            Browser_Back = VK_BROWSER_BACK,
            Browser_Forward = VK_BROWSER_FORWARD,
            Browser_Refresh = VK_BROWSER_REFRESH,
            Browser_Stop = VK_BROWSER_STOP,
            Browser_Search = VK_BROWSER_SEARCH,
            Browser_Favorities = VK_BROWSER_FAVORITES,
            Browser_Home = VK_BROWSER_HOME,
            Volume_Mute = VK_VOLUME_MUTE,
            Volume_Down = VK_VOLUME_DOWN,
            Volume_Up = VK_VOLUME_UP,
            Next_Track = VK_MEDIA_NEXT_TRACK,
            Previous_Track = VK_MEDIA_PREV_TRACK,
            Media_Stop = VK_MEDIA_STOP,
            Media_Play_Pause = VK_MEDIA_PLAY_PAUSE,
            Mail_Start = VK_LAUNCH_MAIL,
            App1_Start = VK_LAUNCH_APP1,
            App2_Start = VK_LAUNCH_APP2,
            //0xB8 - 0xB9 Reserved
            OEM_1 = VK_OEM_1,//In US Standard Keyboard it is ';:'
            Plus = VK_OEM_PLUS,
            Comma = VK_OEM_COMMA,
            Minus = VK_OEM_MINUS,
            Period = VK_OEM_PERIOD,
            OEM_2 = VK_OEM_2,//In US Stadard Keyboard it is '/?'
            OEM_3 = VK_OEM_3,//in US Standard Keyboard it is '~'
            //0xC1 - 0xD7 Reserved
            //0xD8 - 0xDA Unassigned
            OEM_4 = VK_OEM_4,//In US Standard Keyboard it is '[{'
            OEM_5 = VK_OEM_5,//In US Standard Keyboard it is '\|'
            OEM_6 = VK_OEM_6,//In US Standard Keyboard it is ']}'
            OEM_7 = VK_OEM_7,//In US Standard Keyboard it is '"
            OEM_8 = VK_OEM_8,
            //0xE0 Reserved
            //0xE1 OEM specific
            OEM_102 = VK_OEM_102,//In US Standard Keyboard it is '<>' or the '\\|' in the non-US 102-key keyboard
            Process = VK_PROCESSKEY,//IME Process key
            //0xE6 OEM specific
            Packet = VK_PACKET,//Used to pass Unicode characters as if they were keystrokes.
            //0xE8 Unassigned
            //0xE9 - 0xF5 OEM specific
            Attn = VK_ATTN,
            CrSel = VK_CRSEL,
            ExSel = VK_EXSEL,
            EraseEOF = VK_EREOF,
            Play = VK_PLAY,
            Zoom = VK_ZOOM,
            //0xFC Reserved
            PA1 = VK_PA1,
            Clear_OEM = VK_OEM_CLEAR
#endif
        }VirtualKey;
    };
    
}
#endif