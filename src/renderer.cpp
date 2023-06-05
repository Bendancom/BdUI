#include <renderer.hpp>

namespace BdUI {
    bool Renderer::IsLoadOpenGL = false;

#ifdef _WIN32
    Renderer::Renderer(HWND hWnd) {
        this->hWnd = hWnd;
        Initialize();
    }
    Renderer::Renderer(HDC hDC) {
        this->hDC = hDC;
        Initialize();
    }
#endif

    Renderer::~Renderer() {
        join();
        delete Renderer_Thread;
#ifdef _WIN32
        //wglDeleteContext(hRC);
        if(hWnd != nullptr) ReleaseDC(hWnd, hDC);
#endif
    }
    void Renderer::Render(Delegate<void()> d) {
        Push(std::move(d));
    }

    void Renderer::join() {
        Mutex.lock();
        Mutex.unlock();
    }

    void Renderer::operator()(Delegate<void()> d) {
        Push(std::move(d));
    }

    void Renderer::Initialize() {
#ifdef _WIN32
        if (hDC == nullptr) hDC = GetDC(hWnd);
        int&& render = ChoosePixelFormat(hDC, &Pfd);
        if (render == 0) throw error::Class::Initialize_Failed("PixelFormat initialization faild");
        /*
        if (hRC == 0) {
            Pfd.dwFlags &= ~PFD_DOUBLEBUFFER;
            Pfd.dwFlags |= PFD_SUPPORT_GDI;
            int&& ren = ChoosePixelFormat(hDC, &Pfd);
            if (ren == 0) throw error::Class::Initialize_Failed("PixelFormat initialization faild");
            SetPixelFormat(hDC, ren, &Pfd);
            DescribePixelFormat(hDC, ren, sizeof(PIXELFORMATDESCRIPTOR), &Pfd);
            hRC = wglCreateContext(hDC);
        }*/
        Renderer_Thread = new std::thread(&Renderer::RenderMessageLoop, this);
        Renderer_Thread->detach();
        Initialization.get_future().get();
#endif
    }

    void Renderer::RenderMessageLoop() {
        Initialization.set_value(true);
        std::unique_lock<std::mutex> unique_lock(Mutex);
        while (true) {
            if (RendeMessage_Queue.size() == 0) condition.wait(unique_lock);
            else {
                RendeMessage_Queue.front()();
                RendeMessage_Queue.pop();
            }
        }
    }
}