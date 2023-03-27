#include <math/bezier.hpp>

namespace BdUI {
	/*const Parametricfunc Bezier::bezier_func{
		{ 'x',Meta("n! / (n-i)! / i! * t^i * (1-t)^{n-i} * x_i") },
		{ 'y',Meta("n! / (n-i)! / i! * t^i * (1-t)^{n-i} * y_i") },
		{ 'z',Meta("n! / (n-i)! / i! * t^i * (1-t)^{n-i} * z_i") }
	};
	*/
	std::array<std::pair<Character, double>, 3> Bezier::calculate(double t) const {
		std::map<Variable,long double>&& temp{ {Variable("i"),0},{Variable("n"),this->size() - 1}};
		int k = 0;
		for (Point p : *this) {
			temp[Variable{'x', { k }}] = p.X;
			temp[Variable{'y', { k }}] = p.Y;
			temp[Variable{'z', { k }}] = p.Z;
			k++;
		}
		temp[Variable("t")] = t;
		std::vector<std::pair<Variable,long double>> result = bezier_func.calculate(temp);
		return { 
			std::pair<Character,double>{result[0].first.first,result[0].second},
			std::pair<Character,double>{result[1].first.first,result[1].second},
			std::pair<Character,double>{result[2].first.first,result[2].second}
		};
	}
}