#ifndef BDUI_MONITOR
#define BDUI_MONITOR

#ifdef _WIN32
#include <Windows.h>
#endif
#include <array>
#include <error.hpp>

namespace BdUI {
	class Window;
	class Monitor {
	private:
#ifdef _WIN32
		HMONITOR monitor = nullptr;
#endif
	public:
		Monitor();
		Monitor(const long long& Pixel_X,const long long& Pixel_Y);
		#ifdef _WIN32
		Monitor(HWND);
		#endif
		std::array<long double, 2> GetDPI() const;
		static unsigned char GetMonitorNumber();
	};
}

#endif