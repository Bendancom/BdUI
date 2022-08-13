#include "graph/size.hpp"
#include <initializer_list>

namespace BdUI {
	Size::Size(const std::array<double,2>& list) {
		Width = *list.data();
		Height = *(list.data() + 1);
	}
	void Size::ChangeUnit(const UnitType::UnitType& type) {
		if (type == UnitType::PixelHorizon || type == UnitType::PixelVertical) throw error::Function::ParamError();
		if (type == Type) return;
		if (type == UnitType::Pixel) {
			Width = Unit(Width, Type).GetData(UnitType::PixelHorizon);
			Height = Unit(Height, Type).GetData(UnitType::PixelVertical);
			Type = type;
		}
		else {
			Width = Unit(Width, Type).GetData(type);
			Height = Unit(Height, Type).GetData(type);
			Type = type;
		}
	}
	UnitType::UnitType Size::GetType() const {
		return Type;
	}
	std::array<double, 2> Size::GetData(UnitType::UnitType type) const {
		return { Unit(Width,Type).GetData(type),Unit(Height,Type).GetData(type) };
	}

	Size& Size::operator=(const std::array<double,2>& list) {
		Width = *list.data();
		Height = *(list.data() + 1);
		return *this;
	}
	Size& Size::operator=(const Size& size) {
		Width = size.Width;
		Height = size.Height;
		Type = size.Type;
		return *this;
	}

	Size& Size::operator+(Size& p) {
		p.ChangeUnit(Type);
		Width += p.Width;
		Height += p.Height;
		return *this;
	}
	Size& Size::operator+=(Size& p) {
		p.ChangeUnit(Type);
		Width += p.Width;
		Height += p.Height;
		return *this;
	}
	Size& Size::operator-(Size& p) {
		p.ChangeUnit(Type);
		Width -= p.Width;
		Height -= p.Height;
		return *this;
	}
	Size& Size::operator-=(Size& p) {
		p.ChangeUnit(Type);
		Width -= p.Width;
		Height -= p.Height;
		return *this;
	}
}