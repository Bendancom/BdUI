#include <window.hpp>

namespace BdUI {
    bool Window::SetTitleText(std::string new_string, std::string*& old_string) {
        if (old_string == nullptr) old_string = new std::string(new_string);
        else *old_string = new_string;
#ifdef _WIN32
        if(hWnd != nullptr) SetWindowText(hWnd, new_string.c_str());
#endif
        return true;
    }

    bool Window::SetHotKey(std::vector<BdUI::Key> l, std::vector<BdUI::Key>*& l_p) {

        return true;
    }
    bool Window::SizeChange(BdUI::Size size, BdUI::Size*& old) {
        if (old == nullptr) old = new BdUI::Size(size);
        else *old = size;
        size.ChangeUnit(UnitType::Pixel);
#ifdef WIN32
        if (hWnd != nullptr) SetWindowPos(hWnd, NULL, 0, 0, size.Width, size.Height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_ASYNCWINDOWPOS);
#endif
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
        RGBA rgb = n.GetRGBA();
        Render.PushMessage(glClearColor,float(rgb.R) / 255, float(rgb.G) / 255, float(rgb.B) / 255, float(rgb.A) / 255);
        GraphChanged = true;
        return true;
    }
    bool Window::SetVSync(bool n, bool*& old) {
        if (old == nullptr) old = new bool(n);
        else *old = n;
#ifdef _WIN32
        Render.PushMessage(wglSwapIntervalEXT,n);
#endif
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
        Render.PushMessage(glViewport, 0, 0, size.Width, size.Height);
        return true;
    }

    bool Window::SetIcon(BdUI::Icon icon, BdUI::Icon*& old) {
        if (old == nullptr) old = new BdUI::Icon(icon);
        else *old = icon;
#ifdef _WIN32
        if (hWnd != nullptr) SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)icon.getIndex());
#endif
        return true;
    }
}