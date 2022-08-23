#include "window.hpp"

using namespace BdUI;
using namespace std;

std::string Error() {
	DWORD error = GetLastError();
	LPVOID str;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&str, 0, NULL);
	char* s = reinterpret_cast<char*>(str);
	return s;
}

int main()
{
	std::cout << sizeof(std::list<Meta*>)<<std::endl;
	std::cout << sizeof(std::vector<char>) << std::endl;
	Window window;
	bool create = window.Create();
	window.Block();
	return 0;
}