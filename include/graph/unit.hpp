#ifndef BDUI_UNIT
#define BDUI_UNIT

#include <cmath>
#include "error.hpp"

#ifdef WIN32
#include <Windows.h>
#endif


namespace BdUI {
	enum UnitType {
		Pixel = 0,
		PixelHorizon = 1,
		PixelVertical = 2,
		inch = 3,
		mm = 4,
		cm = 5,
		dm = 6,
		m = 7,
		Unknown = 8,
	};

	class Unit{
	private:
		double Number = 0;
		bool NumberExist = false;
		UnitType unitType;
		unsigned short Power;
		struct {
			short X;
			short Y;
		}DPI;
		void GetDPI();
	public:
		Unit(const long double& n,const UnitType& u = Unknown,const unsigned short& power = 1) : Number(n), unitType(u), NumberExist(true),Power(power) {}
		Unit(const Unit& u) : Number(u.Number), NumberExist(u.NumberExist),unitType(u.unitType) {}

		void ChangeUnit(const UnitType&);		

		double Output(const UnitType&);
		double Output();
		void Input(const long double&, const UnitType&);

		operator double();

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