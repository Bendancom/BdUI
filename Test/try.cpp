#include "window.hpp"

using namespace std;
using namespace BdUI;

int main(int argv,char** args)
{
	Icon a("C:\\Users\\施昊光\\Downloads\\favicon.png");
	Cursor b("C:\\Users\\施昊光\\Downloads\\favicon.png");
	a.LoadFromFile();
	a.Resize({ 64,64 });
	b.LoadFromFile();
	b.Resize({ 64,64 });
	b.SetHotPoint({ 32,32,0 });
	Window window;
	window.Create();
	window.Icon = a;
	window.Cursor.Client = b;
	window.Visible = true;
	window.Block();
	return 0;
}