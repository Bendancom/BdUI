#ifndef BDUI_MOUSE
#define BDUI_MOUSE

#include <graph/point.hpp>

namespace BdUI {
    struct MouseButton{
        unsigned Left : 1;
        unsigned Right : 1;
        unsigned Middle : 1;
        unsigned X1 : 1;
        unsigned X2 : 1;
        unsigned Ctrl : 1;
        unsigned Shift : 1;
        unsigned Alt : 1;
    };

    struct Mouse {
        Point Location = Point(0,0, UnitType::px);
        //Up = 0, Down = 1;
        MouseButton Button{ 0,0,0,0,0,0,0,0 };
        enum MouseContent {
            Leave = 0,
            Move = 1,
            Hover = 2
        }Content;
        int WheelDelta = 0;
    };
}

#endif