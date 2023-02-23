#include "window.hpp"

using namespace std;
using namespace BdUI;

Window window;

void AAAA() {
	printf("込込込込込込");
}

int main(int argv,char** args)
{
	/*
	std::shared_ptr<Image> i(new Image("C:\\Users\\仏蟷高\\Downloads\\favicon.png"));
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

	cout << sizeof(Function) << endl;
	cout << sizeof(Meta) << endl;

	Meta a = 1;
	a += 1;
	std::list<std::pair<Calculation, Meta>>::const_iterator iter = a.content.begin();
	a.erase(iter);
	cout << a() << endl;
	window.Create();
	window.PopMenuKey.push_back(KeyList{Control,M});
	window.PopMenuKey.push_back(RButton);
	window.Visible = true;
	window.Block();
	return 0;
}