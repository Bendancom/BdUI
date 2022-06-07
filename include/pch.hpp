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
#endif

#include "attribute.hpp"
#include "delegate.hpp"
#include "event.hpp"
#include "readonly.hpp"
#include "resource.hpp"

namespace BdUI
{
    struct Point{
        int X = 0;
        int Y = 0;
    };
    struct Size{
        int Width = 0;
        int Height = 0;
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
        }Button = None;
        int WheelDelta = 0;
    };
    
}
#endif