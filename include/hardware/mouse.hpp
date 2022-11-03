#ifndef BDUI_MOUSE
#define BDUI_MOUSE

#include <graph/point.hpp>

namespace BdUI {
    struct Mouse {
        Point Location = { 0,0,0 };
        struct {
            unsigned Left : 1;
            unsigned Right : 1;
            unsigned Middle : 1;
            unsigned X1 : 1;
            unsigned X2 : 1;
            unsigned Ctrl : 1;
            unsigned Shift : 1;
            unsigned Alt : 1;
        }Button{ 0,0,0,0,0 };    //Up = 0, Down = 1;
        struct {
            unsigned IsLeaved : 1;  // false = 0, true = 1
            unsigned Hover_Move : 1;  //Hover = 0, Move = 1
            unsigned Client_Border : 1; //Client =0, Border = 1
        }Content{ 0,0,0 };
        int WheelDelta = 0;
    };
}

#endif