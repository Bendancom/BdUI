#include "graph/unit.hpp"

namespace BdUI {
	void Unit::ChangeUnit(const UnitType& u) {
		if (u == unitType || u == Unknown) return;
		if (unitType == Unknown) unitType = u;
		if (!NumberExist) {
			unitType = u;
			return;
		}
		if (u >= UnitType::mm && u <= UnitType::m && unitType >= mm && unitType <= m) {
			long long&& power = pow(pow(10, Power),abs(u - unitType));
			if (u - unitType <= 0) Number = Number * power;
			else Number = Number / power;
			unitType = u;
		}
		else {
			switch (u) {
				case PixelHorizon: {
					if (DPI.X == 0) GetDPI();
					if (unitType == inch) {
						Number *= DPI.X;
						unitType = u;
					}
					else if (unitType == PixelVertical) unitType = u;
					else {
						long long&& power = pow(pow(10, Power), abs(cm - unitType));
						if (cm - unitType <= 0) Number = Number * power;
						else Number = Number / power;
						Number *= DPI.X / 2.54;
						unitType = u;
					}
					break;
				}
				case PixelVertical: {
					if (DPI.Y == 0) GetDPI();
					if (unitType == inch) {
						Number *= DPI.Y;
						unitType = u;
					}
					else if (unitType == PixelHorizon) unitType = u;
					else {
						long long&& power = pow(pow(10, Power), abs(cm - unitType));
						if (cm - unitType <= 0) Number = Number * power;
						else Number = Number / power;
						Number *= DPI.Y / 2.54;
						unitType = u;
					}
					break;
				}
				case inch: {
					if (unitType == PixelHorizon) Number /= DPI.X;
					else if (unitType == PixelVertical) Number /= DPI.Y;
					else {
						long long&& power = pow(pow(10, Power), abs(cm - unitType));
						if (cm - unitType <= 0) Number = Number * power;
						else Number = Number / power;
						Number /= 2.54;
					}
					break;
				}
			}
		}
	}

	void Unit::GetDPI() {
#ifdef WIN32
		SetProcessDPIAware();
		HDC desktop = GetDC(NULL);
		DPI.X = GetDeviceCaps(desktop, LOGPIXELSX);
		DPI.Y = GetDeviceCaps(desktop, LOGPIXELSY);
#endif
	}
	double Unit::Output() {
		return Number;
	}
	double Unit::Output(const UnitType& u) {
		Unit unit(*this);
		unit.ChangeUnit(u);
		return unit.Output();
	}
	void Unit::Input(const long double& n, const UnitType& u) {
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