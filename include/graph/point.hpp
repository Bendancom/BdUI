#ifndef BDUI_POINT
#define BDUI_POINT

#include "unit.hpp"
#include <initializer_list>

namespace BdUI {
	class Point {
	private:
		UnitType _Type = Unknown;
	public:
		double X = 0;
		double Y = 0;
		const UnitType& Type = _Type;

		Point(){}
		Point(std::initializer_list<double>);
		Point(const double& x,const double& y,const UnitType& type = Unknown) : X(x),Y(y),_Type(type) {}
		
		void ChangeUnit(const UnitType& type);

		Point& operator=(std::initializer_list<double>);
		Point& operator=(const Point&);

		Point& operator+(Point&);
		Point& operator+=(Point&);
		Point& operator-(Point&);
		Point& operator-=(Point&);
	};
}
#endif