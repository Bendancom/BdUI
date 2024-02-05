#include <window.hpp>

#include <iostream>

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

	for(const auto& i : avaiable){
		std::cout<< i.layerName << endl;
	}
	for(const auto& i : Extention){
		std::cout<< i.extensionName << endl;
	}
	window.Create();
	window.PopMenuKey.push_back(KeyList{Control,M});
	window.PopMenuKey.push_back(RButton);
	window.Visible = true;
	window.Block();
	return 0;
}