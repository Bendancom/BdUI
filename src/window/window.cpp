#include <window.hpp>

namespace BdUI{
    std::map < HWND, Window*> Window::WindowList;
    bool Window::IsLoadOpenGL = false;
    Window::Window(){
        WindowEventDefaultBind();
        WindowCursorDefaultBind();

        Size = BdUI::Size(1000, 800, UnitType::Pixel);
        Location = Point(5, 5, 0, UnitType::cm);
        Focus = this;
    }
    Window::~Window(){
        delete Thread;
    }
    bool Window::Create(){
        Thread = new std::thread(&Window::WindThread,this);
        Thread->detach();
        if(!Creation.get_future().get()){
            delete Thread;
            Thread = nullptr;
            return false;
        }
        return true;
    }
    void Window::Block(){
        Mutex.lock();
        Mutex.unlock();
    }
}