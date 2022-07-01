#include "window.hpp"
#include <iostream>
using namespace BdUI;
using namespace std;

int main()
{
	Window window;
	window.Create();
	window.Show();
	printf("%d : %d\n", window.Location.get().X, window.Location.get().Y);
	printf("%d : %d\n", window.Size.get().Width, window.Size.get().Height);
	window.Block();
	return 0;
}