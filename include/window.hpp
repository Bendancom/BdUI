#include "pch.hpp"

#ifndef BDUI_WINDOW
#define BDUI_WINDOW
namespace BdUI
{
    #ifdef _WIN32
    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    const HINSTANCE hInstance = GetModuleHandle(0);
    #endif

    const std::string ClassName = "BdUI_WindowClass";

    class Window{
    public:
        Window(std::string,Point,Size);
        ~Window();
        Attribute<std::string> Name;            //窗体名
        Attribute<bool> Border = true;          //窗体是否有边框
        Attribute<bool> Caption = true;         //窗体是否有标题栏
        Attribute<bool> Visible = true;         //窗体是否可见
        Attribute<bool> ContextHelp = false;    //窗体标题栏是否有 '?' 帮助
        Attribute<Size> Size;                   //窗体大小
        Attribute<Point> Location;              //窗体位置(以左上角为窗体基点)
        Attribute<bool> DragFile = false;       //窗体是否可以接受拖拽文件    
        Attribute<Cursor> Cursor;               //窗体光标
        #ifdef _WIN32
        HWND hWnd;
        Attribute<HICON&> Icon = Wndclass.hIcon;        //窗体图标
        Attribute<HICON&> IconSm = Wndclass.hIconSm;    //窗体标题栏图标
        
        Attribute<HMENU> Menu;
        Attribute<HBRUSH&> BackgroundColor = Wndclass.hbrBackground;
        Attribute<STRING&> MenuName = Wndclass.lpszMenuName;
        #endif
        void Rendering();
    private:
        #ifdef _WIN32
        WNDCLASSEX Wndclass;
        std::thread *Thread;
        HGLRC OpenGL_Context = nullptr;
        Attribute<int> dwstyle = WS_OVERLAPPEDWINDOW;
        Attribute<int> dwExstyle = 0;
        #endif
        void MessageLoop();
        void Initializatoin();
    };

    #ifdef _WIN32
    static std::map<HWND,Window*> WindowList;
    #endif
}
#endif