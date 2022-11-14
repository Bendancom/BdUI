#include "window.hpp"

using namespace std;
using namespace BdUI;

class AA {
public:
	AA(){}
	~AA(){}
	void B(int a) {
		printf("%d", a);
	}
};

int main()
{
	BdUI::Window window;
	window.Create();
	window.Visible = true;
	window.Key.ChangedEvent;
	window.Block();
	return 0;
}