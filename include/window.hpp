#ifndef BDUI_WINDOW
#define BDUI_WINDOW
#include "pch.hpp"
#include "ui.hpp"

namespace BdUI{
    class Window : public UI{
    public:
        Window();
        ~Window();
        bool Create();
        void Block();
        void Show();
        void Hide();
        
        Attribute<Icon> SmIcon;
        Attribute<Icon> Icon;


        Attribute<Cursor> CaptionCursor;
        Attribute<Cursor> CloseCursor;
        Attribute<Cursor> SizeCursor;
        Attribute<Cursor> ZoomCursor;
        Attribute<Cursor> ReduceCursor;
        Attribute<Cursor> HelpCursor;
        Attribute<Cursor> SysMenuCursor;

    private:
        std::thread *Thread;
        std::mutex Mutex;
        void WindowDefaultEventBind();
        void WindowCursorDefaultBind();

        UI* MouseContext = this;
        Cursor CurrentCursor = ClientCursor;
        
        #ifdef _WIN32
        int dwExStyle = NULL;
        int dwStyle = WS_OVERLAPPEDWINDOW;
        HWND hWnd;
        std::promise<bool> Creation;
        static LRESULT CALLBACK __WndProc(HWND,UINT,WPARAM,LPARAM);
        static LRESULT MouseProc(HWND, UINT, WPARAM, LPARAM, Window*);
        void WindThread();
        #endif
    };
}
#endif