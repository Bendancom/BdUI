#include "window.hpp"
#include <iostream>
using namespace BdUI;

int main()
{
    Window window;
    window.CaptionName = "HHaaaaaa";
    Size size = window.Size;
    Point loca = window.Location;
    
    window.Create();
    window.Show();
    window.Block();
    exit(0);
}