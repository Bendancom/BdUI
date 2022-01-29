#include "pch.hpp"

namespace BdUI
{
    #ifdef _WIN32
    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    const HINSTANCE hInstance = GetModuleHandle(0);
    template<typename HandleType>
    class Resource : public Attribute<HandleType,HandleType,STRING>{
    public:
        using Attribute<HandleType,HandleType,STRING>::Attribute;
        using Attribute<HandleType,HandleType,STRING>::operator=;
    private:
        bool Set(HandleType &h,STRING& s){
            if (typeid(HandleType) == typeid(HICON)) h = LoadIcon(hInstance,s);
            if (typeid(HandleType) == typeid(HCURSOR)) h = LoadCursor(hInstance,s);
            if (typeid(HandleType) == typeid(HBITMAP)) h = LoadBitmap(hInstance,s);
            if (typeid(HandleType) == typeid(HMENU)) h = LoadMenu(hInstance,s);
            return true;
        }
    };
    template<typename HandleType>
    class Resource<HandleType&> : public Attribute<HandleType&,HandleType,STRING>{
    public:
        using Attribute<HandleType&,HandleType,STRING>::Attribute;
        using Attribute<HandleType&,HandleType,STRING>::operator=;
    private:
        bool Set(HandleType &h,STRING& s){
            if (typeid(HandleType) == typeid(HICON)) h = LoadIcon(hInstance,s);
            if (typeid(HandleType) == typeid(HCURSOR)) h = LoadCursor(hInstance,s);
            if (typeid(HandleType) == typeid(HBITMAP)) h = LoadBitmap(hInstance,s);
            if (typeid(HandleType) == typeid(HMENU)) h = LoadMenu(hInstance,s);
            return true;
        }
    };
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
        #ifdef _WIN32
        HWND hWnd = nullptr;
        Resource<HICON&> Icon = Wndclass.hIcon;         //窗体图标
        Resource<HICON&> IconSm = Wndclass.hIconSm;     //窗体标题栏图标
        Resource<HCURSOR&> Cursor = Wndclass.hCursor;   //窗体光标
        Resource<HMENU> Menu;
        Attribute<HBRUSH&> BackgroundColor = Wndclass.hbrBackground;
        Attribute<STRING&> MenuName = Wndclass.lpszMenuName;
        #endif
        void Rendering();
    private:
        #ifdef _WIN32
        WNDCLASSEX Wndclass;
        HGLRC OpenGL_Context = nullptr;
        Attribute<int> dwstyle = 0;
        Attribute<int> dwExstyle = WS_EX_LAYERED;
        #endif
        void MessageLoop();
        void Initializatoin();
    };

    #ifdef _WIN32
    std::vector<Window*> WindowsList;
    #endif
}