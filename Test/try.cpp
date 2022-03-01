#include "window.hpp"
#include <iostream>
using namespace BdUI;
using namespace std;

#ifndef BDUI_WINDOW
#endif

int main()
{
   Window window;
   window.Create();
   window.Visible = true;
   window.Block();
   exit(0);
}