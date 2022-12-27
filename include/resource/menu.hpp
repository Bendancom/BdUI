#ifndef BDUI_MENU
#define BDUI_MENU

#include <resource/resource.hpp>
#include <list>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace BdUI {
	class Menu : public Resource{
	private:
		std::list<Menu*> a;
#ifdef _WIN32
		HMENU menu;
#endif
	public:
		Menu() {}
		Menu(const Menu&);
		~Menu();

#ifdef _WIN32

#endif
	};
}

#endif