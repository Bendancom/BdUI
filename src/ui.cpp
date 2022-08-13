#include "ui.hpp"
namespace BdUI
{
    UI::UI(){
        UICursorDefaultBind();
        UIEventDefaultBind();
        Visible = true;
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
        Mouse.set_func = new Delegate<bool(BdUI::Mouse,BdUI::Mouse*&)>(&UI::MouseRelativePos, this);
        Parent.set_func = new Delegate<bool(UI*, UI*&)>(&UI::ParentSet, this);
    }
    void UI::Paint(const Size& size){
        Shape.get().Paint(Location, size);
        std::list<UI*> uiList = UIList;
        if (uiList.size() == 0) return;
        for (UI* i : uiList) {
            i->Paint(size);
        }
    }

    bool UI::MouseRelativePos(BdUI::Mouse get, BdUI::Mouse*& set) {
        Point location = this->Location;
        if (set == nullptr) set = new BdUI::Mouse(); 
        set->Location = { get.Location.X - location.X, get.Location.Y - location.Y };
        return true;
    }
    bool UI::ParentSet(UI* new_parent, UI*& old_parent) {
        old_parent->UIList.getReference()->remove(this);
        old_parent->UIList.unlock();
        new_parent->UIList.getReference()->push_back(this);
        new_parent->UIList.unlock();
        old_parent = new_parent;
        return true;
    }

    UI* UI::SearchUI_NearPos(const Point& p,UI* ui) {
        UI* result = ui;
        return result;
    }

    Cursor UI::Search_Area_Cursor(const Point& p, UI* ui) {
        return ui->Cursor.Client;
    }
}