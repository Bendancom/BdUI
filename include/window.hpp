#include "pch.hpp"

namespace BdUI
{
    #ifdef _WIN32
    const HINSTANCE hInstance = GetModuleHandle(0);
    const std::string WindowClassName = "BdUI_WindowClass";
    template<typename HandleType>
    class Resource : public Attribute<std::string,HandleType,std::string>{
    public:
        using Attribute<std::string,HandleType,std::string>::Attribute;
        using Attribute<std::string,HandleType,std::string>::operator=;
    private:
        HandleType Get(const std::string &s) {
            if (s.empty()) return NULL;
            if (typeid(HandleType) == typeid(HICON)) return (HandleType)LoadIcon(hInstance,s.c_str());
            if (typeid(HandleType) == typeid(HCURSOR)) return (HandleType)LoadCursor(hInstance,s.c_str());
            if (typeid(HandleType) == typeid(HBITMAP)) return (HandleType)LoadBitmap(hInstance,s.c_str());
            if (typeid(HandleType) == typeid(HMENU)) return (HandleType)LoadMenu(hInstance,s.c_str());
        }
    };
    #endif

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
        Resource<HICON> Icon;                   //窗体图标
        Resource<HICON> IconSm;                 //窗体标题栏图标
        Resource<HCURSOR> Cursor = std::string(IDC_ARROW);   //窗体光标
        Resource<HMENU> Menu;
        Attribute<HBRUSH> BackgroundColor = (HBRUSH)COLOR_BACKGROUND;
        Attribute<std::string> MenuName = std::string("Menu");
        static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
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