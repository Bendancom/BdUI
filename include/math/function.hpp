#ifndef BDUI_FUNCTION
#define BDUI_FUNCTION

#include <math/meta.hpp>
#include <list>

namespace BdUI {

	class function : public std::pair <Variable,Meta> {
	public:
		using std::pair<Variable, Meta>::pair;

		std::pair<Variable,long double> calculate(const std::map<Variable,long double>& = std::map<Variable,long double>()) const;
		std::pair<Variable,long double> operator()(const std::map<Variable,long double>& = std::map<Variable,long double>()) const;

		using std::pair<Variable,Meta>::operator=;
	};

	class Parametricfunc : public std::vector<function>{
	public:
		using std::vector<function>::vector;

		std::vector<std::pair<Variable,long double>> calculate(const std::map<Variable,long double>& m = std::map<Variable,long double>()) const;
		std::vector<std::pair<Variable,long double>> operator()(const std::map<Variable,long double>& m = std::map<Variable,long double>()) const;

		using std::vector<function>::operator=;
	};
}

#endif