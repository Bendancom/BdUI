#include "window.hpp"
#include <iostream>
using namespace BdUI;
using namespace std;

int main()
{
	Window window;
	window.Size = BdUI::Size(10, 10, UnitType::cm);
	Attribute<bool> a = true;
	bool b = a;
	window.Create();
	/*
	UI* ui = new UI;
	Shape rect(BdUI::ShapeType::Polygon);
	rect.push_back(Point(-10, 10, UnitType::cm));
	rect.push_back(Point(10, 10, UnitType::cm));
	rect.push_back(Point(10, -10, UnitType::cm));
	rect.push_back(Point(-10, -10, UnitType::cm));
	ui->Shape = rect;
	window.UIList.push_back(ui);
	*/
	printf("%d\n", sizeof(Point));
	printf("%d\n", sizeof(double));
	printf("%d\n", sizeof(UnitType::UnitType));
	printf("%lf : %lf\n", window.Location.get().X, window.Location.get().Y);
	window.Block();
	return 0;
}