#include "window.hpp"

using namespace std;
using namespace BdUI;

Window window;

void AAAA(unsigned int a) {
	printf("%d",a);
}

int main(int argv,char** args)
{
	PopMenu menu;
	MenuItem menu_item;
	menu_item.context = "hahhahaha";
	menu_item.Click_function = AAAA;
	menu.AppendItem(menu_item);

	window.PopMenu = menu;
	window.PopMenuKey.push_back(KeyList(RButton));
	window.Create();
	window.Visible = true;
	window.Block();
	return 0;
}