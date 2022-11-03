#include "window.hpp"

using namespace BdUI;
using namespace std;

int main()
{
	std::cout << sizeof(std::list<Meta*>)<<std::endl;
	std::cout << sizeof(std::vector<char>) << std::endl;
	Window window;
	bool create = window.Create();
	window.Block();
	return 0;
}