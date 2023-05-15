#ifndef BDUI_SIZE
#define BDUI_SIZE

#include "graph/unit.hpp"
#include "hardware/monitor.hpp"
#include <array>

namespace BdUI {
	class Size : public std::array<long double,2>{
	private:
		UnitType Type;
	public:
		Size(const std::array<long double,2>& list,UnitType type = UnitType::px) : std::array<long double, 2>(list),Type(type) {}
		Size(const long double& width, const long double& height, UnitType type = UnitType::px) : std::array<long double, 2>{width,height},Type(type) {}

		long double& Width = this->at(0);
		long double& Height = this->at(1);

		std::array<long double, 2> GetData(UnitType) const;
		UnitType GetType() const;
		std::array<long double, 2> GetPixel(const Monitor&) const;
		void SetData(const std::array<long double, 2>&, UnitType);

		using std::array<long double, 2>::operator=;

		Size& operator=(const Size&);

		Size& operator+(Size&);
		Size& operator+=(Size&);
		Size& operator-(Size&);
		Size& operator-=(Size&);
	};
}
#endif