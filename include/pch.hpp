#ifndef BDUI_PCH
#define BDUI_PCH

#include <string>
#include <typeinfo>
#include <future>
#include <thread>
#include <chrono>
#include <condition_variable>
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
#include "./resource/bitmap.hpp"
#include "./resource/cursor.hpp"

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
        }Button = None;
        int WheelDelta = 0;
    };
    
}
#endif