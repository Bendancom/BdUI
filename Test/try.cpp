#include "window.hpp"
#include <iostream>
using namespace BdUI;
using namespace std;

int main()
{
	Cursor c = Cursor(LoadCursor(NULL, IDC_ARROW));
	Window window;
	window.Create();
	window.Visible = true;
	window.Block();
	return 0;
}