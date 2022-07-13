#include "graph/unit.hpp"

namespace BdUI {
	void Unit::ChangeUnit(const UnitType::UnitType& u) {
		if (u == unitType || u == UnitType::Unknown) return;
		if (unitType == UnitType::Unknown) unitType = u;
		if (!NumberExist) {
			unitType = u;
			return;
		}
		if (u >= UnitType::mm && u <= UnitType::m && unitType >= UnitType::mm && unitType <= UnitType::m) {
			long long&& power = pow(pow(10, Power),abs(u - unitType));
			if (u - unitType <= 0) Number = Number * power;
			else Number = Number / power;
			unitType = u;
		}
		else {
			switch (u) {
				case UnitType::PixelHorizon: {
					if (DPI.X == 0) GetDPI();
					if (unitType == UnitType::inch) {
						Number *= DPI.X;
						unitType = u;
					}
					else if (unitType == UnitType::PixelVertical) unitType = u;
					else {
						long long&& power = pow(pow(10, Power), abs(UnitType::cm - unitType));
						if (UnitType::cm - unitType <= 0) Number = Number * power;
						else Number = Number / power;
						Number *= DPI.X / 2.54;
						unitType = u;
					}
					break;
				}
				case UnitType::PixelVertical: {
					if (DPI.Y == 0) GetDPI();
					if (unitType == UnitType::inch) {
						Number *= DPI.Y;
						unitType = u;
					}
					else if (unitType == UnitType::PixelHorizon) unitType = u;
					else {
						long long&& power = pow(pow(10, Power), abs(UnitType::cm - unitType));
						if (UnitType::cm - unitType <= 0) Number = Number * power;
						else Number = Number / power;
						Number *= DPI.Y / 2.54;
						unitType = u;
					}
					break;
				}
				case UnitType::inch: {
					if (unitType == UnitType::PixelHorizon) Number /= DPI.X;
					else if (unitType == UnitType::PixelVertical) Number /= DPI.Y;
					else {
						long long&& power = pow(pow(10, Power), abs(UnitType::cm - unitType));
						if (UnitType::cm - unitType <= 0) Number = Number * power;
						else Number = Number / power;
						Number /= 2.54;
					}
					break;
				}
			}
		}
	}
	// TODO: 多显示屏DPI问题
	void Unit::GetDPI() {
#ifdef WIN32
		SetProcessDPIAware();
		HDC desktop = GetDC(NULL);
		DPI.X = double(GetDeviceCaps(desktop, HORZRES)) / GetDeviceCaps(desktop, HORZSIZE) * 25.4;
		DPI.Y = double(GetDeviceCaps(desktop, VERTRES)) / GetDeviceCaps(desktop, VERTSIZE) * 25.4;
#endif
	}
	const UnitType::UnitType& Unit::GetType() {
		return unitType;
	}
	double Unit::Output() {
		return Number;
	}
	double Unit::Output(const UnitType::UnitType& u) {
		Unit unit(*this);
		unit.ChangeUnit(u);
		return unit.Output();
	}
	void Unit::Input(const long double& n, const UnitType::UnitType& u) {
		Number = n;
		unitType = u;
	}
	Unit::operator double() {
		return Number;
	}

	Unit& Unit::operator+(Unit& u) {
		u.ChangeUnit(unitType);
		Number += u.Number;
		return *this;
	}
	Unit& Unit::operator-(Unit& u) {
		u.ChangeUnit(unitType);
		Number -= u.Number;
		return *this;
	}
	Unit& Unit::operator*(Unit& u) {
		u.ChangeUnit(unitType);
		Number *= u.Number;
		return *this;
	}
	Unit& Unit::operator/(Unit& u) {
		u.ChangeUnit(unitType);
		Number /= u.Number;
		return *this;
	}
	Unit& Unit::operator+=(Unit& u) {
		u.ChangeUnit(unitType);
		Number += u.Number;
		return *this;
	}
	Unit& Unit::operator-=(Unit& u) {
		u.ChangeUnit(unitType);
		Number -= u.Number;
		return *this;
	}
	Unit& Unit::operator*=(Unit& u) {
		u.ChangeUnit(unitType);
		Number *= u.Number;
		return *this;
	}
	Unit& Unit::operator/=(Unit& u) {
		u.ChangeUnit(unitType);
		Number /= u.Number;
		return *this;
	}
}