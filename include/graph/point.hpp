#ifndef BDUI_POINT
#define BDUI_POINT

#include <graph/unit.hpp>
#include <hardware/monitor.hpp>
#include <array>

namespace BdUI {
	class Point : public std::array<long double, 2>{
	private:
		UnitType Type;
	public:
		Point(const std::array<long double, 2>& array, UnitType type = UnitType::px) : std::array<long double, 2>(array),Type(type) {}
		Point(const long double& x, const long double& y, UnitType type = UnitType::px) : std::array<long double, 2>{x,y},Type(type) {}

		long double& X = this->at(0);
		long double& Y = this->at(1);

		std::array<long double, 2> GetData(UnitType) const;
		std::array<long double, 2> GetPixel(const Monitor& = Monitor()) const;
		UnitType GetType() const;
		void SetData(const std::array<long double, 2>&, UnitType);
		using std::array<long double, 2>::operator=;

		Point& operator=(const Point&);

		Point& operator+(Point&);
		Point& operator+=(Point&);
		Point& operator-(Point&);
		Point& operator-=(Point&);
	};
}
#endif