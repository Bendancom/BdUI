#include "ui.hpp"

namespace BdUI{
    UI::~UI(){
        std::vector<UI*> &uilist = ParentWindow != nullptr ? ParentWindow->UIList : ParentUI->UIList;
        for(std::vector<UI*>::iterator iter = uilist.begin(); iter != uilist.end(); iter++){
            if(*iter == this) uilist.erase(iter);
        }
    }

    bool UI::Create(){
        #ifdef _WIN32
        RegisterClassEx(&UIclass);
        std::pair<int,int> style = Style;
        Point loca = Location.Get();
        BdUI::Size size = Size.Get();
        const HWND &hPWnd = Parent->hWnd;
        hWnd = CreateWindowEx (style.second,(STRING)ClassName.c_str(),NULL,style.first,loca.X,loca.Y,size.Width,size.Height,hPWnd,NULL,hInstance,NULL);
        if (hPWnd == NULL) return false;
        else return true;
        #endif
    }
}