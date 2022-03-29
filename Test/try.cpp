#include "window.hpp"
#include <iostream>
using namespace BdUI;
using namespace std;

int main()
{
   Window window;
   window.Create();
   window.Visible = true;
   window.Block();
   return 0;
}