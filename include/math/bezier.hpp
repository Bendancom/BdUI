#ifndef BDUI_BEZIER
#define BDUI_BEZIER

#include "math/meta.hpp"
#include "math/function.hpp"
#include "graph/point.hpp"

namespace BdUI {
	class Bezier : public std::list<Point> {
	private:
		static const Parametricfunc bezier_func;
	public:
		using std::list<Point>::list;
		using std::list<Point>::operator=;
		std::array<std::pair<Character, double>, 2> calculate(double t) const;
	};
}

#endif