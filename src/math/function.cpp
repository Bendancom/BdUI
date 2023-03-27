#include <math/function.hpp>

namespace BdUI {
	std::pair<Variable,long double> function::calculate(const std::map<Variable,long double>& m) const{
		return { first, second.calculate(m) };
	}
	std::pair<Variable,long double> function::operator()(const std::map<Variable,long double>& m) const{
		return calculate(m);
	}
	std::vector<std::pair<Variable,long double>> Parametricfunc::calculate(const std::map<Variable,long double>& m) const {
		std::vector<std::pair<Variable,long double>>&& temp{};
		for (int i = 0; i < this->size(); i++)
			temp.push_back(at(i).calculate(m));
		return temp;
	}
	std::vector<std::pair<Variable,long double>> Parametricfunc::operator()(const std::map<Variable,long double>& m) const {
		return calculate(m);
	}
}