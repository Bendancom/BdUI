#include "window.hpp"

Window::Window(string classname,string name,int x,int y,int width,int heigth){
    #ifdef _WIN32
    int dwstyle = 0;
    int dw_ex_style = WS_EX_LAYERED;
    if (Style.Border) dwstyle = dwstyle|WS_BORDER;
    if (Style.Caption) dwstyle = dwstyle|WS_CAPTION;
    if (Style.ContextHelp) dw_ex_style = dw_ex_style|WS_EX_CONTEXTHELP;
    if (Style.Visible) dwstyle = dwstyle|WS_VISIBLE;
    if (Style.Border) dw_ex_style =dw_ex_style|WS_EX_ACCEPTFILES;
    
    window = CreateWindowEx (dw_ex_style,(LPCWSTR)classname.c_str(),(LPCWSTR)name.c_str(),
                    dwstyle,x,y,width,heigth,NULL,NULL,GetModuleHandle(NULL),NULL);
    #endif
}