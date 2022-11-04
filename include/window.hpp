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
        
        Attribute<Color> BackgroundColor;
        Attribute<Color> TransparentColor;
        Attribute<Icon> SmIcon;
        Attribute<Icon> Icon;
        Attribute<UI*> Focus;
        Attribute<Size> Size;
        Attribute<bool> VSync;
        

        struct {
            Attribute<BdUI::Cursor> Caption;
            Attribute<BdUI::Cursor> Close;
            Attribute<BdUI::Cursor> Size;
            Attribute<BdUI::Cursor> Zoom;
            Attribute<BdUI::Cursor> Reduce;
            Attribute<BdUI::Cursor> Help;
            Attribute<BdUI::Cursor> SysMenu;
        }WindowCursor;

        AttributeVector<KeyList> HotKey;
        EventArray<KeyList>* HotKey_Tigger;
        
    private:
        std::thread *Thread;
        std::mutex Mutex;
        std::mutex OpenGLMutex;
        static bool IsLoadOpenGL;

        void WindowEventDefaultBind();
        void WindowCursorDefaultBind();

        bool OpenGLLoader();
        
        bool SetHotKey(std::list<BdUI::Key>, std::list<BdUI::Key>*&);
        bool SizeChange(BdUI::Size,BdUI::Size*&);
        bool LocationChange(Point, Point*&);
        bool SetBackgroundColor(Color, Color*&);
        bool SetVSync(bool,bool*&);
        bool SetClientSize(BdUI::Size, BdUI::Size*&);
        bool SetVisible(bool, bool*&);

        UI* MouseContext = this;
        BdUI::Cursor CurrentCursor = Cursor.Client;
        
        bool GraphChanged = true;

        #ifdef _WIN32
        static std::map<HWND,Window*> WindowList;
        int dwExStyle = NULL;
        int dwStyle = WS_OVERLAPPEDWINDOW;
        HWND hWnd = nullptr;
        HDC hDC = nullptr;
        HGLRC hRC = nullptr;
        std::promise<bool> Creation;


        static LRESULT CALLBACK __WndProc(HWND,UINT,WPARAM,LPARAM);
        static LRESULT MouseProc(HWND, UINT, WPARAM, LPARAM, Window*);
        static void MouseVitualKey(WPARAM,BdUI::Mouse&);
        void WindThread();

        const WNDCLASSEX Windowclass{
            sizeof(WNDCLASSEX),
            CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
            Window::__WndProc,
            0,
            8,
            GetModuleHandle(NULL),
            NULL,
            NULL,
            NULL,
            NULL,
            TEXT("BdUI_WindowClass"),
            NULL,
        };
        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
            32,                        //Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                        //Number of bits for the depthbuffer
            8,                        //Number of bits for the stencilbuffer
            0,                        //Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };
        #endif
    };
}
#endif