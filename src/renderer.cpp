#include <renderer.hpp>

namespace BdUI {
    bool Renderer::IsLoadOpenGL = false;

    Renderer::~Renderer() {
        delete Renderer_Thread;
#ifdef _WIN32
        wglDeleteContext(hRC);
        ReleaseDC(hWnd, hDC);
#endif
    }

    void Renderer::Render() {
        PushMessage(&Renderer::_Render, this);
    }
    void Renderer::_Render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFlush();
#ifdef _WIN32
        SwapBuffers(hDC);
#endif
    }


#ifdef _WIN32
	void Renderer::Initialize(HWND hWnd) {
        this->hWnd = hWnd;

        hDC = GetDC(hWnd);
        int error = GetLastError();
        int&& render = ChoosePixelFormat(hDC, &pfd);
        if (render == 0) throw error::Class::Initialize_Failed("PixelFormat initialization faild");
        SetPixelFormat(hDC, render, &pfd);
        hRC = wglCreateContext(hDC);
        if (hRC == 0) throw error::Class::Initialize_Failed("OpenGL Context creation faild");

        Renderer_Thread = new std::thread(&Renderer::RenderMessageLoop,this);
        Renderer_Thread->detach();
        Initialization.get_future().get();
	}
#endif
    void Renderer::RenderMessageLoop() {
#ifdef _WIN32
        wglMakeCurrent(hDC, hRC);
#endif
        if (!IsLoadOpenGL) {
#ifdef _WIN32
            if (!gladLoadWGL(hDC))
                throw error::OpenGL::Initialization_Failed("gladLoadWGL faild");
#endif
            if (!gladLoadGL())
                throw error::OpenGL::Initialization_Failed("gladLoad faild");
            IsLoadOpenGL = true;
        }

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