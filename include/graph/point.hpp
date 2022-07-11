#ifndef BDUI_POINT
#define BDUI_POINT

#include "unit.hpp"
#include <initializer_list>

namespace BdUI {
	class Point {
	private:
		UnitType::UnitType Type = UnitType::Unknown;
	public:
		double X = 0;
		double Y = 0;

		Point(){}
		Point(const std::initializer_list<double>&);
		Point(const double& x,const double& y,const UnitType::UnitType& type = UnitType::Unknown) : X(x),Y(y),Type(type) {}
		
		void ChangeUnit(const UnitType::UnitType& type);
		const UnitType::UnitType& GetType();

		Point& operator=(const std::initializer_list<double>&);
		Point& operator=(const Point&);

		Point& operator+(Point&);
		Point& operator+=(Point&);
		Point& operator-(Point&);
		Point& operator-=(Point&);
	};
}
#endif