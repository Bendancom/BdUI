#ifndef BDUI_PCH
#define BDUI_PCH

#include <string>
#include <typeinfo>
#include <future>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <list>
#include <iostream>
#include <deque>
#include <memory_resource>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "attribute.hpp"
#include "delegate.hpp"
#include "event.hpp"
#include "readonly.hpp"

namespace BdUI
{
    struct Point{
        long X = 0;
        long Y = 0;
    };
    struct Size{
        long Width = 0;
        long Height = 0;
    };
    struct Margin{
        int Left = 0;
        int Right = 0;
        int Top = 0;
        int Bottom = 0;
    };
    struct Mouse{
        Point Location;
        enum Button{
            None = 0,
            Right = 1,
            Left = 2,
            Middle = 3,
            X1 = 4,
            X2 = 5,
        }Button;
        int WheelDelta;
    };
    struct Cursor{
        #ifdef _WIN32
        HCURSOR Client = LoadCursor(NULL,IDC_ARROW);
        HCURSOR Caption = LoadCursor(NULL,IDC_ARROW);;
        HCURSOR LeftBorder = LoadCursor(NULL,IDC_SIZEWE);
        HCURSOR RightBorder = LoadCursor(NULL,IDC_SIZEWE);
        HCURSOR TopBorder = LoadCursor(NULL,IDC_SIZENS);
        HCURSOR BottomBorder = LoadCursor(NULL,IDC_SIZENS);
        HCURSOR BottomLeft = LoadCursor(NULL,IDC_SIZENESW);
        HCURSOR BottomRight = LoadCursor(NULL,IDC_SIZENWSE);
        HCURSOR TopLeft = LoadCursor(NULL,IDC_SIZENWSE);
        HCURSOR TopRight = LoadCursor(NULL,IDC_SIZENESW);
        HCURSOR Close = LoadCursor(NULL,IDC_ARROW);
        HCURSOR Size = LoadCursor(NULL,IDC_ARROW);
        HCURSOR Zoom = LoadCursor(NULL,IDC_ARROW);
        HCURSOR Reduce = LoadCursor(NULL,IDC_ARROW);
        HCURSOR Help = LoadCursor(NULL,IDC_ARROW);
        HCURSOR VScroll = LoadCursor(NULL,IDC_ARROW);
        HCURSOR HScroll = LoadCursor(NULL,IDC_ARROW);
        HCURSOR Menu = LoadCursor(NULL,IDC_ARROW);
        HCURSOR SystemMenu = LoadCursor(NULL,IDC_ARROW);
        HCURSOR Error = LoadCursor(NULL,IDC_ARROW);
        HCURSOR Transparent = LoadCursor(NULL,IDC_ARROW);
        //Windows：https://docs.microsoft.com/zh-cn/windows/win32/inputdev/wm-nchittest
        #endif
    };
}
#endif