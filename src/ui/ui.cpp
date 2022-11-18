#include "ui.hpp"
namespace BdUI
{
    UI::UI(){
        UICursorDefaultBind();
        UIEventDefaultBind();
        Visible = true;
    }
    void UI::Paint(const Size& size){
        Shape.get().Paint(Location, size);
        std::vector<UI*> uiList = UIList;
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
        old_parent->UIList.erase(this);
        new_parent->UIList.push_back(this);
        Parent = new_parent;
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