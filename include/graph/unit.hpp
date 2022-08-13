#ifndef BDUI_UNIT
#define BDUI_UNIT

#include <cmath>
#include "error.hpp"

#ifdef WIN32
#include <Windows.h>
#endif

namespace BdUI {
	namespace UnitType {
		enum UnitType {
			Unknown = 0,
			Pixel = 1,
			inch = 2,
			mm = 3,
			cm = 4,
			dm = 5,
			m = 6,
			PixelHorizon = 7,
			PixelVertical = 8,
		};
	}
	
	class Unit{
	private:
		double Number = 0;
		UnitType::UnitType unitType;
		char Power;
		struct {
			double X = 0;
			double Y = 0;
		}DPI;//Pixel per Millimeter
		void GetDPI();
	public:
		Unit(double n, UnitType::UnitType u = UnitType::Unknown, char power = 1) : Number(n), unitType(u), Power(power) {
			GetDPI();
		}
		Unit(const Unit& u) : Number(u.Number), unitType(u.unitType),Power(u.Power),DPI(u.DPI) {}

		void ChangeUnit(UnitType::UnitType);

		UnitType::UnitType GetType() const;
		char GetPow() const;
		double GetData(UnitType::UnitType) const;

		void SetData(double, UnitType::UnitType);
		void SetMonitor();

		operator double() const;

		Unit& operator+(Unit&);
		Unit& operator-(Unit&);
		Unit& operator*(Unit&);
		Unit& operator/(Unit&);
		Unit& operator+=(Unit&);
		Unit& operator-=(Unit&);
		Unit& operator*=(Unit&);
		Unit& operator/=(Unit&);
	};
}
#endif