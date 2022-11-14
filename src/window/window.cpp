#include <window.hpp>

namespace BdUI{
    std::map < HWND, Window*> Window::WindowList;
    Window::Window(){
        WindowEventDefaultBind();
        WindowCursorDefaultBind();

        Size = BdUI::Size(1000, 800, UnitType::Pixel);
        Location = Point(5, 5, 0, UnitType::cm);
        BackgroundColor = RGB{ 255, 255, 255 };
        Focus = this;
    }
    Window::~Window(){
        delete Thread;
    }
    void Window::Create(){
        Thread = new std::thread(&Window::WindThread,this);
        Thread->detach();
        Creation.get_future().get();
    }
    void Window::Block(){
        Mutex.lock();
        Mutex.unlock();
    }
}