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
		float X = 0;
		float Y = 0;
		float Z = 0;

		Point(){}
		Point(const std::array<float,3>&);
		Point(const float& x,const float& y,const float& z,const UnitType::UnitType& type = UnitType::Unknown) : X(x),Y(y),Z(z), Type(type) {}
		
		Point& ChangeUnit(const UnitType::UnitType&);
		std::array<float, 3> GetData(const UnitType::UnitType&) const;
		UnitType::UnitType GetType() const;
		float* GetPointPtr();

		Point& operator=(const std::array<float,3>&);
		Point& operator=(const Point&);

		Point& operator+(Point&);
		Point& operator+=(Point&);
		Point& operator-(Point&);
		Point& operator-=(Point&);
	};
	typedef Point Vertex;
}
#endif