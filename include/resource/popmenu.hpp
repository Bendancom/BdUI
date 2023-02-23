#ifndef BDUI_MENU
#define BDUI_MENU

#include <resource/resource.hpp>
#include <resource/image/image.hpp>
#include <vector>
#include <algorithm>
#include <delegate.hpp>
#include <memory>
#include <graph/color.hpp>
#include <variant>
#include <graph/point.hpp>
#include <map>
#include <renderer.hpp>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace BdUI {
	class PopMenu;
	enum MenuItem_State {
		Disable = 0,
		Enable = 1,
		Grayed = 2,
	};
	enum MenuItem_Layout{
		None = 0,
		Break = 1,
		BarBreak = 2,
	};
	struct MenuItem {
		std::shared_ptr<Image> checked_image = nullptr;
		std::shared_ptr<Image> unchecked_image = nullptr;
		MenuItem_State state = MenuItem_State::Enable;
		MenuItem_Layout layout = MenuItem_Layout::None;
		bool IsHiLite = false;
		std::variant<std::shared_ptr<PopMenu>, Delegate<void()>> carry_out;
		std::variant<std::shared_ptr<BdUI::Image>, std::string,std::monostate> context;
	};
	struct MenuInfo {
		std::variant<BdUI::Color, std::shared_ptr<BdUI::Image>> background = BdUI::Color(RGBA{255,255,255,0});
		unsigned short MaxHeight = 0;// 0 is unlimit
		enum X_Align {
			X_Center = 0,
			X_Left = 1,
			X_Right = 2,
		} x_align = X_Left;
		enum Y_Align {
			Y_Center = 0,
			Y_Top = 1,
			Y_Bottom = 2,
		} y_align = Y_Top;
		enum RelativeLocation {
			ScreenPosition = 0,
			MousePosition = 1,
			WindowPosition = 2,
		} relative = MousePosition;
		Point Origin = {0,0,0};
	};
	

	class PopMenu : public Resource{
	private:
		
		std::vector<MenuItem> ItemList;
		MenuInfo menuinfo;
#ifdef _WIN32
		static std::map<UINT, MenuItem*> PopMenuMap;
		std::map<std::size_t,UINT> ID;
		HMENU menu = nullptr;
		void CreateHMenu();
		MENUITEMINFO TransformMenuItemInfo(unsigned int pos);
#endif
	public:
		PopMenu();
		PopMenu(const std::filesystem::path&);
		PopMenu(const PopMenu&);
		~PopMenu();

		PopMenu& AppendItem(const MenuItem& item);
		PopMenu& InsertItem(unsigned int pos,const MenuItem& item);
		PopMenu& AppendItems(const std::vector<MenuItem>& items);
		PopMenu& InsertItems(unsigned int pos,const std::vector<MenuItem>& items);
		PopMenu& SetMenuItem(unsigned int pos,MenuItem item);
		PopMenu& SetMenuInfo(const MenuInfo&);
		MenuInfo GetMenuInfo();


#ifdef _WIN32
		void DrawItem(UINT);
		PopMenu(HMENU);
		operator HMENU();
		HMENU getIndex();
		void setIndex(HMENU);
		PopMenu& operator=(HMENU);
		void PopUp(HWND);
#endif
		PopMenu& operator=(const PopMenu&);
	};
}
#endif