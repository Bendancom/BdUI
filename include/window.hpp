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
        Attribute<UI*> Focus = this;

        struct {
            Attribute<BdUI::Cursor> Caption;
            Attribute<BdUI::Cursor> Close;
            Attribute<BdUI::Cursor> Size;
            Attribute<BdUI::Cursor> Zoom;
            Attribute<BdUI::Cursor> Reduce;
            Attribute<BdUI::Cursor> Help;
            Attribute<BdUI::Cursor> SysMenu;
        }WindowCursor;
        
    private:
        std::thread *Thread;
        std::mutex Mutex;
        static bool IsLoadOpenGL;

        void WindowEventDefaultBind();
        void WindowCursorDefaultBind();
        
        bool WindowSizeChange(const Point*,BdUI::Size,BdUI::Size&);
        bool WindowSetText(const std::string&);
        bool WindowLocationChange(const BdUI::Size*, Point, Point&);

        void Paint();

        UI* MouseContext = this;
        const BdUI::Cursor* CurrentCursor = Cursor.Client;
        
        #ifdef _WIN32
        int dwExStyle = NULL;
        int dwStyle = WS_OVERLAPPEDWINDOW;
        HWND hWnd;
        HDC hDC;
        HGLRC hRC;
        std::promise<bool> Creation;
        static LRESULT CALLBACK __WndProc(HWND,UINT,WPARAM,LPARAM);
        static LRESULT MouseProc(HWND, UINT, WPARAM, LPARAM, Window*);
        static void MouseVitualKey(WPARAM,BdUI::Mouse&);
        void WindThread();
        #endif
    };
}
#endif