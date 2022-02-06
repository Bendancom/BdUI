#include "window.hpp"
#include <iostream>
using namespace BdUI;

int main()
{
    Window window;
    window.CaptionName = "HHaaaaaa";
    window.BackgroundColor = (HBRUSH)COLOR_BACKGROUND;
    window.Create();
    window.Show();
    window.Block();
    exit(0);
}