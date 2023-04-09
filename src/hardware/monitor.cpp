#include <hardware/monitor.hpp>

namespace BdUI {
	unsigned char Monitor::GetMonitorNumber() {
#ifdef _WIN32
		return GetSystemMetrics(SM_CMONITORS);
#endif
	}
	std::array<long double, 2> Monitor::GetDPI() const{
#ifdef _WIN32
		MONITORINFOEX info{};
		info.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(monitor, &info);
		DEVMODE dm{};
		dm.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &dm);
		return { (long double)(info.rcMonitor.right - info.rcMonitor.left) / dm.dmPelsWidth,(long double)(info.rcMonitor.bottom - info.rcMonitor.top) / dm.dmPelsHeight };
#endif
	}
	Monitor::Monitor() {
#ifdef _WIN32
		monitor = (HMONITOR)GetDC(NULL);
#endif
	}
	Monitor::Monitor(const long long& x, const long long& y) {
#ifdef _WIN32
		POINT&& p{(long)x,(long)y};
		monitor = MonitorFromPoint(p, MONITOR_DEFAULTTONULL);
		if (monitor == NULL) throw error::Monitor::point_error();
#endif
	}
	Monitor::Monitor(const Window& window) {
#ifdef _WIN32
		monitor = MonitorFromWindow(window.hWnd, MONITOR_DEFAULTTONULL);
		if (monitor == NULL) throw error::Monitor::window_error();
#endif
	}
}