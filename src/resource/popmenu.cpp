#include <resource/popmenu.hpp>

namespace BdUI {
	std::map<UINT, MenuItem*> PopMenu::PopMenuMap;

	PopMenu::PopMenu() {
		Resource_Type = Resource::PopMenu;
	}
	PopMenu::PopMenu(const std::filesystem::path& str) {
		Resource_Type = Resource::PopMenu;
		FilePath = str;
	}
	PopMenu::PopMenu(const PopMenu& m) {
		Resource_Type = Resource::PopMenu;
		ItemList = m.ItemList;
#ifdef _WIN32
		CreateHMenu();
#endif
	}
	PopMenu::~PopMenu() {
#ifdef _WIN32
		for (int i = 0; i < ItemList.size(); i++)
			RemoveMenu(menu, i, MF_BYPOSITION);
		DestroyMenu(menu);
		for (size_t i = 0; i < ItemList.size(); i++)
			PopMenuMap.erase(PopMenuMap.find(ID[i]));
#endif
	}

	void PopMenu::PopUp(HWND hWnd) {
#ifdef _WIN32
		UINT flags = TPM_RETURNCMD | TPM_NONOTIFY;
		switch (menuinfo.x_align)
		{
		case MenuInfo::X_Center:
			flags |= TPM_CENTERALIGN;
			break;
		case MenuInfo::X_Left:
			flags |= TPM_LEFTALIGN;
			break;
		case MenuInfo::X_Right:
			flags |= TPM_RIGHTALIGN;
			break;
		}
		switch (menuinfo.y_align)
		{
		case MenuInfo::Y_Center:
			flags |= TPM_VCENTERALIGN;
			break;
		case MenuInfo::Y_Top:
			flags |= TPM_TOPALIGN;
			break;
		case MenuInfo::Y_Bottom:
			flags |= TPM_BOTTOMALIGN;
			break;
		}
		std::array<float, 3>p = menuinfo.Origin.GetData(UnitType::Pixel);
		switch (menuinfo.relative)
		{
		case MenuInfo::ScreenPosition: {
			break;
		}
		case MenuInfo::WindowPosition: {
			RECT rect;
			GetWindowRect(hWnd, &rect);
			p[0] += rect.left;
			p[1] += rect.top;
			break;
		}
		case MenuInfo::MousePosition: {
			POINT point;
			GetCursorPos(&point);
			p[0] += point.x;
			p[1] += point.y;
			break;
		}
		}
		if (UINT id = TrackPopupMenuEx(menu, flags, p[0], p[1], hWnd, nullptr); id > 0) {
			std::get<Delegate<void()>>(PopMenuMap[id]->carry_out)();
		}
#endif
	}

	PopMenu& PopMenu::AppendItem(const MenuItem& item) {
		ItemList.push_back(item);
#ifdef _WIN32
		CreateHMenu();
#endif
		return *this;
	}
	PopMenu& PopMenu::InsertItem(unsigned int pos, const MenuItem& item) {
		if (pos <= ItemList.size()) {
			std::vector<MenuItem>::iterator iter = ItemList.begin() + pos;
			ItemList.insert(iter, item);
		}
		else throw error::Function::ParamError("Position not find");
#ifdef _WIN32
		CreateHMenu();
#endif
		return *this;
	}
	PopMenu& PopMenu::AppendItems(const std::vector<MenuItem>& items) {
		ItemList.insert(ItemList.end(), items.begin(), items.end());
#ifdef _WIN32
		CreateHMenu();
#endif
		return *this;
	}
	PopMenu& PopMenu::InsertItems(unsigned int pos, const std::vector<MenuItem>& items) {
		if (pos <= ItemList.size()) {
			std::vector<MenuItem>::iterator iter = ItemList.begin() + pos;
			ItemList.insert(iter, items.begin(), items.end());
		}
		else throw error::Function::ParamError("Position not find");
#ifdef _WIN32
		CreateHMenu();
#endif
		return *this;
	}

	PopMenu& PopMenu::SetMenuItem(unsigned int pos, MenuItem item) {
		if (pos < ItemList.size()) {
			ItemList[pos] = item;
#ifdef _WIN32
			MENUITEMINFO info = TransformMenuItemInfo(pos);
			SetMenuItemInfo(menu, pos, true, &info);
#endif
		}
		else throw error::Function::ParamError("position bigger than the itmes number");
		return *this;
	}

#ifdef _WIN32
	void PopMenu::DrawItem(UINT id) {
		if (auto color = std::get_if<Color>(&menuinfo.background); color) {
			GLRGBA c = color->GetGLRGBA();
			glClearColor(c.R, c.G, c.B, c.A);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else {

		}
	}

	void PopMenu::CreateHMenu() {
		if (menu != nullptr) {
			for (int i = 0; i < ItemList.size(); i++)
				RemoveMenu(menu, i, MF_BYPOSITION);
			DestroyMenu(menu);
		}
		menu = CreatePopupMenu();

		SetMenuInfo(menuinfo);

		for (size_t id = 0; id < ItemList.size(); id++) {
			MENUITEMINFO info = TransformMenuItemInfo(id);
			InsertMenuItem(menu, id, true, &info);
		}
	}

	MENUITEMINFO PopMenu::TransformMenuItemInfo(unsigned int pos) {
		const MenuItem& item = ItemList.at(pos);
		MENUITEMINFO&& menuiteminfo = { 0 };
		menuiteminfo.cbSize = sizeof(MENUITEMINFO);
		
		menuiteminfo.fMask = MIIM_STATE | MIIM_DATA;
		menuiteminfo.dwItemData = reinterpret_cast<ULONG_PTR>(&item);
		if (auto i = std::get_if<std::shared_ptr<PopMenu>>(&item.carry_out);i != nullptr) {
			menuiteminfo.fMask |= MIIM_SUBMENU;
			menuiteminfo.hSubMenu = i->get()->getIndex();
		}
		else {
			menuiteminfo.hSubMenu = NULL;
			menuiteminfo.fMask |= MIIM_ID | MIIM_FTYPE;
			UINT id = 1;
			while (PopMenuMap.find(id) != PopMenuMap.end()) {
				id = rand() + 1;
			}
			if (ID.find(pos) != ID.end()) {
				PopMenuMap.erase(PopMenuMap.find(ID[pos]));
				ID.erase(ID.find(pos));
			}
			PopMenuMap[id] = const_cast<MenuItem*>(&item);
			ID[pos] = id;
			menuiteminfo.wID = id;
		}
		menuiteminfo.fType = MFT_OWNERDRAW;
		if (auto i = std::get_if<std::shared_ptr<BdUI::Image>>(&item.context); i) {
			menuiteminfo.fMask |= MIIM_BITMAP;
			menuiteminfo.hbmpItem = i->get()->getIndex();
		}
		else if (auto i = std::get_if<std::string>(&item.context); i) {
			menuiteminfo.fMask |= MIIM_STRING;
			menuiteminfo.dwTypeData = const_cast<char*>(i->c_str());
			menuiteminfo.cch = i->size();
		}
		else {
			menuiteminfo.fType = MFT_SEPARATOR;
		}
		
		switch (item.layout)
		{
		case MenuItem_Layout::None: break;
		case MenuItem_Layout::Break: {
			menuiteminfo.fType |= MFT_MENUBREAK;
			break;
		}
		case MenuItem_Layout::BarBreak: {
			menuiteminfo.fType |= MFT_MENUBARBREAK;
			break;
		}
		}

		if (item.checked_image != nullptr) {
			menuiteminfo.fState |= MFS_CHECKED;
			menuiteminfo.hbmpChecked = item.checked_image->getIndex();
		}
		if (item.unchecked_image != nullptr) {
			menuiteminfo.fState |= MFS_UNCHECKED;
			menuiteminfo.hbmpUnchecked = item.unchecked_image->getIndex();
		}
		menuiteminfo.fState |= item.IsHiLite ? MFS_HILITE : MFS_UNHILITE;
		switch (item.state)
		{
		case MenuItem_State::Disable: {
			menuiteminfo.fState |= MFS_DISABLED;
			break;
		}
		case MenuItem_State::Enable: {
			menuiteminfo.fState |= MFS_ENABLED;
			break;
		}
		case MenuItem_State::Grayed: {
			menuiteminfo.fState |= MFS_GRAYED;
			break;
		}
		}
		return menuiteminfo;
	}

	PopMenu& PopMenu::SetMenuInfo(const MenuInfo& info) {
		menuinfo = info;
#ifdef _WIN32
		MENUINFO minfo = {
			sizeof(MENUINFO),
			MIM_STYLE | MIM_MAXHEIGHT | MIM_MENUDATA,
			MNS_DRAGDROP,
			info.MaxHeight,
			0,0,
			reinterpret_cast<ULONG_PTR>(this)
		};
		::SetMenuInfo(menu,&minfo);
#endif
		return *this;
	}

	MenuInfo PopMenu::GetMenuInfo() {
		return menuinfo;
	}

	PopMenu::operator HMENU() {
		if (menu == nullptr) throw error::Class::Uninitialize();
		return menu;
	}

	PopMenu::PopMenu(HMENU m) {
		Resource_Type = Resource::PopMenu;
		menu = m;
	}
	PopMenu& PopMenu::operator=(HMENU m) {
		menu = m;
		return *this;
	}
	HMENU PopMenu::getIndex() {
		if (menu == nullptr) throw error::Class::Uninitialize();
		return menu;
	}
	void PopMenu::setIndex(HMENU m) {
		menu = m;
	}
#endif

	PopMenu& PopMenu::operator=(const PopMenu& m) {
		ItemList = m.ItemList;
		menuinfo = m.menuinfo;
#ifdef _WIN32
		CreateHMenu();
#endif
		return *this;
	}
}