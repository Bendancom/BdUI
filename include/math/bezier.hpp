#ifndef BDUI_BEZIER
#define BDUI_BEZIER

#include <math/function.hpp>
#include <graph/point.hpp>

namespace BdUI {
	class Bezier : public std::list<Point> {
	private:
		Parametricfunc bezier_func;
	public:
		Bezier(const std::initializer_list<Point>&);
		using std::list<Point>::operator=;
		std::array<std::pair<Character, double>, 3> calculate(double t) const;
	};
}

#endif