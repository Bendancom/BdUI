#include "ui.hpp"

namespace BdUI{
    UI::~UI(){
        #ifdef _WIN32
        DestroyWindow(hWnd);
        #endif
    }
}