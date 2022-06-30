#include "ui.hpp"
namespace BdUI
{
    UI::UI(){
        UICursorDefaultBind();
        UIEventDefaultBind();
    }
    void UI::UICursorDefaultBind(){
#ifdef WIN32
        Cursor.Client = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Menu = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Border.Left = BdUI::Cursor(LoadCursor(NULL, IDC_SIZEWE));
        Cursor.Border.Right = BdUI::Cursor(LoadCursor(NULL, IDC_SIZEWE));
        Cursor.Border.Top = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENS));
        Cursor.Border.Bottom = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENS));
        Cursor.Border.BottomLeft = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENESW));
        Cursor.Border.BottomRight = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENWSE));
        Cursor.Border.TopLeft = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENWSE));
        Cursor.Border.TopRight = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENESW));
#endif
    }
    void UI::UIEventDefaultBind() {
        Mouse.set_func = new Delegate<bool(BdUI::Mouse,BdUI::Mouse&)>(&UI::MouseRelativePos, this, Location.getPointer(), std::placeholders::_1, std::placeholders::_2);
    }

    bool UI::MouseRelativePos(const Point* location, BdUI::Mouse get, BdUI::Mouse& set) {
        set.Location = { get.Location.X - location->X,get.Location.Y - location->Y };
        return true;
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

    const Cursor* UI::Search_Area_Cursor(const Point& p, UI* ui) {
        return ui->Cursor.Client.getPointer();
    }
}