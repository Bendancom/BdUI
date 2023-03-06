#include "window.hpp"

using namespace std;
using namespace BdUI;

Window window;

int main(int argv, char** args)
{
	/*
	std::shared_ptr<Image> i(new Image("C:\\Users\\ʩ껹�\\Downloads\\favicon.png"));
	i->LoadFromFile();
	PopMenu menu;
	MenuItem item;
	item.context = "hahahahaha";
	item.carry_out = AAAA;
	MenuInfo info;
	info.background = Color(RGBA{ 0,0,0,0 });
	menu.AppendItem(item);
	menu.SetMenuInfo(info);
	window.PopMenu = menu;*/

	Bezier b{ {0,0,0},{1,0,1},{0,0,2} };
	auto c = b.calculate(0.5);
	window.Create();
	window.PopMenuKey.push_back(KeyList{Control,M});
	window.PopMenuKey.push_back(RButton);
	window.Visible = true;
	window.Block();
	return 0;
}