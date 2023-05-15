#ifndef BDUI_POINT
#define BDUI_POINT

#include <graph/unit.hpp>
#include <hardware/monitor.hpp>
#include <array>

namespace BdUI {
	class Point{
	private:
		UnitType Type;
	public:
		Point(const std::array<long double, 2>& arr, UnitType type = UnitType::px) : X(arr[0]),Y(arr[1]), Type(type) {}
		Point(const long double& x, const long double& y, UnitType type = UnitType::px) : X(x),Y(y),Type(type) {}

		long double X;
		long double Y;

		std::array<long double, 2> GetData(UnitType) const;
		std::array<long double, 2> GetPixel(const Monitor& = Monitor()) const;
		UnitType GetType() const;
		void SetData(long double x,long double y, UnitType);
		void SetData(const std::array<long double,2>&, UnitType);

		Point& operator=(const Point&) = default;

		long double operator[](unsigned int);
		Point& operator+(Point&);
		Point& operator+=(Point&);
		Point& operator-(Point&);
		Point& operator-=(Point&);
	};
}
#endif