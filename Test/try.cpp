#include "window.hpp"
#include <iostream>
using namespace BdUI;
using namespace std;

int main()
{
	Window window;
	window.Create();
	window.Show();
	window.Block();
	return 0;
}