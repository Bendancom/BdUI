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
        void SetText(const std::string&);
        
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
        void WindowDefaultEventBind();
        void WindowCursorDefaultBind();
        

        template<typename E, typename... T>
        static void EventDeliver(UI*, T...) requires(typeid(E) == typeid(EventArray<void(T...)>));

        UI* MouseContext = this;
        const BdUI::Cursor* CurrentCursor = Cursor.Client;
        
        #ifdef _WIN32
        int dwExStyle = NULL;
        int dwStyle = WS_OVERLAPPEDWINDOW;
        HWND hWnd;
        std::promise<bool> Creation;
        static LRESULT CALLBACK __WndProc(HWND,UINT,WPARAM,LPARAM);
        static LRESULT MouseProc(HWND, UINT, WPARAM, LPARAM, Window*);
        static void MouseVitualKey(WPARAM,BdUI::Mouse&);
        void WindThread();
        #endif
    };
}
#endif