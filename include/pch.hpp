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
        struct{
            unsigned Button : 3;
            /*  None = 0,
                Right = 1,
                Left = 2,
                Middle = 3,
                X1 = 4,
                X2 = 5, */
            unsigned Up_Down : 1;   //Up = 0, Down = 1
            unsigned IsLeaved : 1;  // false = 0, true = 1
            unsigned Hover_Move : 1;  //Hover = 0, Move = 1
            unsigned Client_Border : 1; //Client =0, Border = 1
        }Content;
        int WheelDelta = 0;
    };
    
}
#endif