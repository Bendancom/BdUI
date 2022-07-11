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
        Parent.set_func = new Delegate<bool(UI*, UI*&)>(&UI::ParentSet, this);
    }
    void UI::Paint(const Size& size) {
        Shape.get().Paint(Location,size);
        for (auto i : UIList) {
            i->Paint(size);
        }
    }

    bool UI::MouseRelativePos(const Point* location, BdUI::Mouse get, BdUI::Mouse& set) {
        set.Location = { get.Location.X - location->X,get.Location.Y - location->Y };
        return true;
    }
    bool UI::ParentSet(UI* ui, UI*& set) {
        set->UIList.remove(this);
        ui->UIList.push_back(this);
        set = ui;
        return true;
    }

    UI* UI::SearchUI_NearPos(const Point& p,UI* ui) {
        UI* result = ui;
        return result;
    }

    const Cursor* UI::Search_Area_Cursor(const Point& p, UI* ui) {
        return ui->Cursor.Client.getPointer();
    }
}