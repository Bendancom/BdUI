#include "window.hpp"

using namespace std;
using namespace BdUI;

int main()
{
	Window window;
	bool create = window.Create();
	window.BackgroundColor = RGB{ 0,0,0 };
	window.Block();
	return 0;
}