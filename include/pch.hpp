#ifndef BDUI_PCH
#define BDUI_PCH

#include <string>
#include <typeinfo>
#include <future>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <semaphore>
#include <list>
#include <glad/glad.h>
#ifdef _WIN32
#include <windows.h>
#include <glad/glad_wgl.h>
#ifdef UNICODE
#define STRING LPCWSTR
#else
#define STRING LPCSTR
#endif
#endif

#include "attribute.hpp"
#include "delegate.hpp"
#include "event.hpp"

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
    
    struct Style{
        #ifdef _WIN32
        //Windows Style
        enum WindowType{
        Window = 0,
        PopUp = 1,
        Child = 2,
        LayerdWindow = 3
        }Type = Window;
        bool Border = true;
        bool Caption = true;
        bool Disable = false;
        bool HScroll = false;
        bool VScroll = false;
        bool MinSizeInit = false;
        bool MaxSizeInit = false;
        bool Size = true;           //从属于SysMenu
        bool MaxSize = true;        //从属于SysMenu
        bool MinSize = true;        //从属于SysMenu
        bool SysMenu = true;        //从属于Caption
        bool Tabs_Top = false;
        bool Visible = false;
        bool ClipChildren = true;

        //Windows ExStyle
        bool AcceptFiles = false;
        bool ContextHelp = false;    //不与MaxSizeBox或MinSizeBox共存
        #endif
    };//WindowStyle Default

    const std::string ClassName = "BdUI_WindowClass";
    #ifdef _WIN32
    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    std::pair<int,int> AttributeGetStyle(const Style&);
    #endif
}
#endif