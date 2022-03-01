#include "ui.hpp"
namespace BdUI
{
    UI::UI(){
        UIEventBind();
        MouseHoverTime = 1000;
    }

    void UI::UIEventBind(){
        Size.EventList += SizeChanged;
        Location.EventList += LocationChanged;
        Cursor.EventList += CursorChanged;
        Visible.EventList += VisibleChanged;
    }
}