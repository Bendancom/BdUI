#ifndef BDUI_SIZE
#define BDUI_SIZE
#include "unit.hpp"
#include <array>

namespace BdUI {
	class Size {
	private:
		UnitType::UnitType Type = UnitType::Unknown;
	public:
		double Width = 0;
		double Height = 0;

		Size() {}
		Size(const std::array<double,2>&);
		Size(const double& width, const double& height, const UnitType::UnitType& type = UnitType::Unknown) : Width(width), Height(height), Type(type) {}

		Size& ChangeUnit(const UnitType::UnitType& type);
		UnitType::UnitType GetType() const;
		std::array<double, 2> GetData(UnitType::UnitType) const;

		Size& operator=(const std::array<double,2>&);
		Size& operator=(const Size&);

		Size& operator+(Size&);
		Size& operator+=(Size&);
		Size& operator-(Size&);
		Size& operator-=(Size&);
	};
}
#endif