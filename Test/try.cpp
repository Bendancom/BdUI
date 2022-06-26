#include "window.hpp"
#include <iostream>
using namespace BdUI;
using namespace std;

int main()
{
	Cursor c = Cursor(LoadCursor(NULL, IDC_ARROW));
	Window window;
	window.Create();
	window.Show();
	window.Block();
	return 0;
}