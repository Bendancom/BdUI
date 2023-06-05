#ifndef BDUI_RENDER
#define BDUI_RENDER

#ifdef _WIN32
#include <Windows.h>
#endif
#include "ui.hpp"
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <functional>
#include <error.hpp>
#include <atomic>

namespace BdUI {
    class Window;
	class Renderer {
	public:
#ifdef _WIN32
        Renderer(HWND hWnd);
        Renderer(HDC hDC);
#endif
        ~Renderer();

        template<typename gl_Func,typename... Args>
        void Push(gl_Func gl_func, Args... args) {
            Mutex.lock();
            if (IsLoadOpenGL) {
                RendeMessage_Queue.push(std::bind(gl_func, args...));
                condition.notify_all();
            }
            //else throw error::OpenGL::UnInitialization();
            Mutex.unlock();
        }
        void join();
        void Render(Delegate<void()> render_func);
        void operator()(Delegate<void()>);
	private:
        static bool IsLoadOpenGL;
        std::promise<bool> Initialization;
        std::mutex Mutex;
        std::condition_variable condition;
        std::queue<std::function<void(void)>> RendeMessage_Queue;
        std::thread* Renderer_Thread = nullptr;

        void RenderMessageLoop();
        void Initialize();



#ifdef _WIN32
        HWND hWnd = nullptr;
		HDC hDC = nullptr;
        PIXELFORMATDESCRIPTOR Pfd{
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_GENERIC_FORMAT | PFD_GENERIC_ACCELERATED,    //Flags
            PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
            32,                        //Colordepth of the framebuffer.
            8, 0, 
            8, 0, 
            8, 0,
            8,
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