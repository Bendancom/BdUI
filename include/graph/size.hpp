#ifndef BDUI_SIZE
#define BDUI_SIZE
#include "unit.hpp"
#include <initializer_list>

namespace BdUI {
	class Size {
	private:
		UnitType::UnitType Type = UnitType::Unknown;
	public:
		double Width = 0;
		double Height = 0;

		Size() {}
		Size(const std::initializer_list<double>&);
		Size(const double& width, const double& height, const UnitType::UnitType& type = UnitType::Unknown) : Width(width), Height(height), Type(type) {}

		void ChangeUnit(const UnitType::UnitType& type);
		const UnitType::UnitType& GetType();

		Size& operator=(const std::initializer_list<double>&);
		Size& operator=(const Size&);

		Size& operator+(Size&);
		Size& operator+=(Size&);
		Size& operator-(Size&);
		Size& operator-=(Size&);
	};
}
#endif