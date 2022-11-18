#include "window.hpp"

using namespace std;
using namespace BdUI;

int main()
{
	Window window;
	window.Create();
	window.Visible = true;
	window.Block();
	return 0;
}