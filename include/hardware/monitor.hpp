#ifndef BDUI_MONITOR
#define BDUI_MONITOR

#ifdef _WIN32
#include <Windows.h>
#endif
#include <array>
#include <graph/point.hpp>
#include <window.hpp>

namespace BdUI {
	class Monitor {
	private:
#ifdef _WIN32
		HMONITOR monitor = nullptr;
#endif
	public:
		//Ö÷ÆÁÄ»
		Monitor();
		Monitor(const long long& Pixel_X,const long long& Pixel_Y);
		Monitor(const Window&);
		std::array<long double, 2> GetDPI() const;
		static unsigned char GetMonitorNumber();
	};
}

#endif