#include "graph/size.hpp"
#include <initializer_list>

namespace BdUI {
	std::array<long double, 2> Size::GetData(UnitType type) const {
		return { Unit(Width,Type).GetData(type),Unit(Width,Type).GetData(type)};
	}
	UnitType Size::GetType() const { return Type; }
	std::array<long double, 2> Size::GetPixel(const Monitor& monitor) const {
		std::array<long double, 2> dpi = monitor.GetDPI();
		return { Unit::ToUnit(Width,Type,UnitType::inch) * dpi[0],Unit::ToUnit(Height,Type,UnitType::inch) * dpi[1] };
	}

	Size& Size::operator=(const Size& size) {
		Width = size.Width;
		Height = size.Height;
		Type = size.Type;
	}

	void Size::SetData(const std::array<long double, 2>& list, UnitType type) {
		Width = list[0];
		Height = list[1];
		Type = type;
	}
	Size& Size::operator+(Size& p) {
		Width += Unit::ToUnit(p.Width,p.Type,Type);
		Height += Unit::ToUnit(p.Height,p.Type,Type);
		return *this;
	}
	Size& Size::operator+=(Size& p) {
		Width += Unit::ToUnit(p.Width, p.Type, Type);
		Height += Unit::ToUnit(p.Height, p.Type, Type);
		return *this;
	}
	Size& Size::operator-(Size& p) {
		Width -= Unit::ToUnit(p.Width, p.Type, Type);
		Height -= Unit::ToUnit(p.Height, p.Type, Type);
		return *this;
	}
	Size& Size::operator-=(Size& p) {
		Width -= Unit::ToUnit(p.Width, p.Type, Type);
		Height -= Unit::ToUnit(p.Height, p.Type, Type);
		return *this;
	}
}