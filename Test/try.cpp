#include "window.hpp"

using namespace std;

int main()
{
	BdUI::Window window;
	bool create = window.Create();
	window.Visible = true;
	window.Block();
	return 0;
}