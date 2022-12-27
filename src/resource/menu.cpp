#include <resource/menu.hpp>

namespace BdUI {
	Menu::~Menu() {
#ifdef _WIN32
		DestroyMenu(menu);
#endif
	}


}