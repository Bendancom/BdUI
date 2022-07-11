#ifndef BDUI_REFERENCE
#define BDUI_REFERENCE

#include <map>
#include <algorithm>
#include <atomic>

namespace BdUI {
	template<typename T>
	class Reference {
	private:
		T* Value;
		static std::map<void*,std::atomic<short>> Map;
	public:
		Reference(const T& t) {
			bool is = false;
			for (auto i : Map) {
				if (*reinterpret_cast<T*>(i.first) == t) {
					Value = i.first;
					i.second++;
					is = true;
					break;
				}
			}
			if (!is) {
				Value = new T(t);
				Map.insert(std::pair<void*, std::atomic<short>>(Value, 1));
			}
		}
		Reference(const Reference<T>& r) {
			Value = r.Value;
			Map[Value]++;
		}
		Reference<T>& operator=(const T& t) {
			bool is = false;
			for (auto i : Map) {
				if (*reinterpret_cast<T*>(i.first) == t) {
					Value = i.first;
					i.second++;
					is = true;
					break;
				}
			}
			if (!is) {
				Value = new T(t);
				Map.insert(std::pair<void*, std::atomic<short>>(Value, 1));
			}
		}
	};
}
#endif