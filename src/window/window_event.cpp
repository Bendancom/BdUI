#include <window.hpp>
#define OpenGLLock(x) if(IsLoadOpenGL){OpenGLMutex.lock();wglMakeCurrent(hDC, hRC);x;wglMakeCurrent(NULL,NULL);OpenGLMutex.unlock();}

namespace BdUI {
    bool Window::SetHotKey(std::list<BdUI::Key> l, std::list<BdUI::Key>*& l_p) {
        return true;
    }
    bool Window::SizeChange(BdUI::Size size, BdUI::Size*& old) {
        if (old == nullptr) old = new BdUI::Size(size);
        else *old = size;
        size.ChangeUnit(UnitType::Pixel);
#ifdef WIN32
        if (hWnd != nullptr) SetWindowPos(hWnd, NULL, 0, 0, size.Width, size.Height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_ASYNCWINDOWPOS);
#endif
        OpenGLLock(glViewport(0, 0, size.Width, size.Height))
        return true;
    }
    bool Window::LocationChange(Point location, Point*& old) {
        if (old == nullptr) old = new BdUI::Point(location);
        else *old = location;
        location.ChangeUnit(UnitType::Pixel);
#ifdef WIN32
        if (hWnd != nullptr) SetWindowPos(hWnd, NULL, location.X, location.Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_ASYNCWINDOWPOS);
#endif
        return true;
    }
    bool Window::SetBackgroundColor(Color n, Color*& old) {
        if (old == nullptr) old = new BdUI::Color(n);
        else *old = n;
        RGB rgb = n.GetRGB();
        OpenGLLock(glClearColor(float(rgb.R) / 255, float(rgb.G) / 255, float(rgb.B) / 255, float(n.GetAlpha()) / 255))
        return true;
    }
    bool Window::SetVSync(bool n, bool*& old) {
#ifdef _WIN32
        OpenGLLock(wglSwapIntervalEXT(n))
#endif
        if (old == nullptr) old = new bool(n);
        else *old = n;
        return true;
    }
    bool Window::SetVisible(bool visible, bool*& old) {
        if (old == nullptr) old = new bool(visible);
        else *old = visible;
        if (hWnd != nullptr) {
            if (visible) {
#ifdef _WIN32
                ShowWindow(hWnd, SW_SHOW);
                UpdateWindow(hWnd);
#endif
            }
            else {
#ifdef _WIN32
                ShowWindow(hWnd, SW_HIDE);
#endif
            }
        }
        return true;
    }
    bool Window::SetClientSize(BdUI::Size size, BdUI::Size*& old) {
        if (old == nullptr) old = new BdUI::Size(size);
        else *old = size;
        OpenGLLock(glViewport(0, 0, size.Width, size.Height))
        return true;
    }
}