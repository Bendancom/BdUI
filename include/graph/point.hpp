#ifndef BDUI_POINT
#define BDUI_POINT

#include "unit.hpp"
#include <array>
#include <initializer_list>

namespace BdUI {
	class Point {
	private:
		UnitType::UnitType Type = UnitType::Unknown;
	public:
		double X = 0;
		double Y = 0;
		double Z = 0;

		Point(){}
		Point(const std::array<double,3>&);
		Point(const double& x,const double& y,const double& z,const UnitType::UnitType& type = UnitType::Unknown) : X(x),Y(y),Z(z), Type(type) {}
		
		void ChangeUnit(const UnitType::UnitType&);
		std::array<double, 3> GetPoint(const UnitType::UnitType&);
		const UnitType::UnitType& GetType();

		Point& operator=(const std::array<double,3>&);
		Point& operator=(const Point&);

		Point& operator+(Point&);
		Point& operator+=(Point&);
		Point& operator-(Point&);
		Point& operator-=(Point&);
	};
}
#endif