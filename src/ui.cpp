#include "ui.hpp"
namespace BdUI
{
    UI::UI(){
        UICursorDefaultBind();
    }
    void UI::UICursorDefaultBind(){
#ifdef WIN32
        ClientCursor = Cursor(LoadCursor(NULL, IDC_ARROW));
        BorderCursor_Left = Cursor(LoadCursor(NULL, IDC_SIZEWE));
        BorderCursor_Right = Cursor(LoadCursor(NULL, IDC_SIZEWE));
        BorderCursor_Top = Cursor(LoadCursor(NULL, IDC_SIZENS));
        BorderCursor_Bottom = Cursor(LoadCursor(NULL, IDC_SIZENS));
        BottomLeftCursor = Cursor(LoadCursor(NULL, IDC_SIZENESW));
        BottomRightCursor = Cursor(LoadCursor(NULL, IDC_SIZENWSE));
        TopLeftCursor = Cursor(LoadCursor(NULL, IDC_SIZENWSE));
        TopRightCursor = Cursor(LoadCursor(NULL, IDC_SIZENESW));
        MenuCursor = Cursor(LoadCursor(NULL, IDC_ARROW));
#endif
    }

    UI* UI::SearchUI_NearPos(const Point& p,UI* ui) {
        UI* result = ui;
        for (UI* i : ui->UIList)
        {
            Point&& location = i->Location;
            BdUI::Size&& size = i->Size;
            if (p.X >= location.X && p.X <= location.X + size.Width && p.Y >= location.Y && p.Y <= location.Y + size.Height) {
                if (i->UIList.size() == 0) return i;
                else result = SearchUI_NearPos(p, i);
            }
        }
        return result;
    }

    Cursor UI::Search_Area_Cursor(const Point& p, UI* ui) {
        return ui->ClientCursor.get();
    }
}