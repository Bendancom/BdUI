#include <string>
#include <typeinfo>
#include <glad/glad.h>
#ifdef _WIN32
#include <windows.h>
#include <glad/glad_wgl.h>
#endif

#include "attribute.hpp"
#include "delegate.hpp"
#include "event.hpp"

#ifdef UNICODE
#define STRING LPCWSTR
#else
#define STRING LPCSTR
#endif



#ifndef BDUI_PCH
#define BDUI_PCH
namespace BdUI
{
    struct Point{
        Point(long x = 0,long y = 0) : X(x),Y(y) {}
        long X;
        long Y;
    };
    struct Size{
        Size(long w = 0,long h = 0) : Width(w),Heigth(h) {}
        long Width;
        long Heigth;
    };
    struct Cursor{
        #ifdef _WIN32
        HCURSOR Client;
        HCURSOR Caption;
        HCURSOR LeftBorder;
        HCURSOR RightBorder;
        HCURSOR TopBorder;
        HCURSOR BottomBorder;
        HCURSOR BottomLeft;
        HCURSOR BottomRight;
        HCURSOR TopLeft;
        HCURSOR TopRight;
        HCURSOR Close;
        HCURSOR Size;
        HCURSOR GrowBox;//与Size相同
        HCURSOR Zoom;
        HCURSOR Reduce;
        HCURSOR Help;
        HCURSOR VScroll;
        HCURSOR HScroll;
        HCURSOR Menu;
        HCURSOR SystemMenu;
        HCURSOR Error;
        HCURSOR Transparent;
        #endif
        //对照表：https://docs.microsoft.com/zh-cn/windows/win32/inputdev/wm-nchittest
    };
}
#endif