#include "graph/unit.hpp"

namespace BdUI {
	double Unit::GetData(UnitType::UnitType u) const {
		int&& number = Number;
		if (u == unitType || u == UnitType::Unknown) return number;
		else if (u >= UnitType::mm && u <= UnitType::m) {
			if (unitType >= UnitType::mm && unitType <= UnitType::m)
				number *= pow(pow(10, unitType - u), Power);
			else if (unitType == UnitType::PixelHorizon)
				number /= DPI.X * pow(pow(10, UnitType::mm - u), Power);
			else if (unitType == UnitType::PixelVertical)
				number /= DPI.Y * pow(pow(10, UnitType::mm - u), Power);
			else if (unitType == UnitType::inch)
				number /= 2.54 * pow(pow(10, UnitType::cm - u), Power);
		}
		else if (u == UnitType::PixelHorizon) {
			if (unitType >= UnitType::mm && unitType <= UnitType::m)
				number *= pow(pow(10, unitType - UnitType::mm), Power) * DPI.X;
			else if (unitType == UnitType::inch)
				number *= pow(pow(10, unitType - UnitType::mm), Power) * DPI.X / 25.4;
			else if (unitType == UnitType::PixelVertical)
				throw error::Function::ParamError();
		}
		else if (u == UnitType::PixelVertical) {
			if (unitType >= UnitType::mm && unitType <= UnitType::m)
				number *= pow(pow(10, unitType - UnitType::mm), Power) * DPI.Y;
			else if (unitType == UnitType::inch)
				number *= pow(pow(10, unitType - UnitType::mm), Power) * DPI.Y / 25.4;
			else if (unitType == UnitType::PixelHorizon)
				throw error::Function::ParamError();
		}
		else if (u == UnitType::inch) {
			if (unitType >= UnitType::mm && unitType <= UnitType::m)
				number *= pow(pow(10, unitType - UnitType::cm), Power) * 2.54;
			else if (unitType == UnitType::PixelHorizon)
				number /= DPI.X * 25.4;
			else if (unitType == UnitType::PixelVertical)
				number /= DPI.Y * 25.4;
		}
		return number;
	}
	void Unit::ChangeUnit(UnitType::UnitType u) {
		Number = GetData(u);
		unitType = u;
	}
	// TODO: 多显示屏DPI问题
	void Unit::GetDPI(){
#ifdef WIN32
		SetProcessDPIAware();
		HDC desktop = GetDC(NULL);
		DPI.X = double(GetDeviceCaps(desktop, HORZRES)) / GetDeviceCaps(desktop, HORZSIZE);
		DPI.Y = double(GetDeviceCaps(desktop, VERTRES)) / GetDeviceCaps(desktop, VERTSIZE);
#endif
	}
	UnitType::UnitType Unit::GetType() const{
		return unitType;
	}
	char Unit::GetPow() const{
		return Power;
	}
	void Unit::SetData(double n, UnitType::UnitType u) {
		Number = n;
		unitType = u;
	}
	Unit::operator double() const{
		return Number;
	}

	Unit& Unit::operator+(Unit& u) {
		Number += u.GetData(unitType);
		return *this;
	}
	Unit& Unit::operator-(Unit& u) {
		Number -= u.GetData(unitType);
		return *this;
	}
	Unit& Unit::operator*(Unit& u) {
		Number *= u.GetData(unitType);
		Power += u.Power;
		if (Power == 0) unitType = UnitType::Unknown;
		return *this;
	}
	Unit& Unit::operator/(Unit& u) {
		Number /= u.GetData(unitType);
		Power -= u.Power;
		if (Power == 0) unitType = UnitType::Unknown;
		return *this;
	}
	Unit& Unit::operator+=(Unit& u) {
		Number += u.GetData(unitType);
		return *this;
	}
	Unit& Unit::operator-=(Unit& u) {
		Number -= u.GetData(unitType);
		return *this;
	}
	Unit& Unit::operator*=(Unit& u) {
		Number *= u.GetData(unitType);
		Power += u.Power;
		if (Power == 0) unitType = UnitType::Unknown;
		return *this;
	}
	Unit& Unit::operator/=(Unit& u) {
		Number /= u.GetData(unitType);
		Power -= Power;
		if (Power == 0) unitType = UnitType::Unknown;
		return *this;
	}
}