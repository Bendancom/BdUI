#ifndef BDUI_FUNCTION
#define BDUI_FUNCTION

#include <math/meta.hpp>
#include <list>

namespace BdUI {

	class function : public Meta {
	public:
		Variable dependent_variable;
		function(const Variable& c,const Meta& m) : dependent_variable(c), Meta(m) {}
		function(const function&);

		std::pair<Variable, double> calculate(const std::map<Variable, double>& = std::map<Variable,double>()) const;
		std::pair<Variable, double> operator()(const std::map<Variable, double>& = std::map<Variable,double>()) const;

		function& operator=(const function&);
		using Meta::operator=;
	};

	class Parametricfunc : public std::vector<function>{
	public:
		using std::vector<function>::vector;

		std::vector<std::pair<Variable, double>> calculate(const std::map<Variable, double>& m = std::map<Variable, double>()) const;
		std::vector<std::pair<Variable, double>> operator()(const std::map<Variable, double>& m = std::map<Variable, double>()) const;

		using std::vector<function>::operator=;
	};
}

#endif