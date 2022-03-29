#include "ui.hpp"
namespace BdUI
{
    UI::UI(){
        MouseHoverTime = 1000;
        CursorDefaultSet();
    }
    void UI::CursorDefaultSet(){
        #ifdef _WIN32
        //Cursor = {IDC_ARROW};
        #endif
    }
}