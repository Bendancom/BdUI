#ifndef BDUI_META
#define BDUI_META

#include <character.hpp> 
#include <string>
#include <list>
#include <variant>
#include <vector>
#include <optional>
#include <map>
#include <cmath>

namespace BdUI{
	enum class Calculation {
		plus,		// 加法运算
		minus,		// 减法运算
		multi,		// 乘法运算
		divide,		// 除法运算
		pow,		// 幂运算
		log,		// 对数运算
	};
	class Variable : public std::pair<Character,std::string>{
	public:
		Variable(const Character& c) : std::pair<Character,std::string>(c,std::string()) {}
		Variable(const char& c) : std::pair<Character,std::string>(c,std::string()) {}
		using std::pair<Character, std::string>::pair;
		using std::pair<Character, std::string>::operator=;
		bool operator<(const Variable&) const;
	};

	class Meta {
	private:
		void Addvariable(const Meta&);
		std::vector<Variable> _variables;
		std::variant<double, Variable> coefficient;
		std::list<std::pair<Calculation,Meta>> Content;
		std::optional<Calculation> operator_calculation;
	public:
		const std::vector<Variable>& variables = _variables;

		Meta(const double& d) : coefficient(d){}
		Meta(const int& i) : coefficient((double)i) {}
		Meta(const char& c) : coefficient(Character(c)) {}
		Meta(const Character&);
		Meta(std::string LATEX_expression);

		double calculate(const std::map<Variable,double>&) const;
		double calculate() const;

		Meta& pow(const Meta&);
		Meta& log(const Meta&);
		Meta& plus(const Meta&);
		Meta& minus(const Meta&);
		Meta& multi(const Meta&);
		Meta& divide(const Meta&);

		Meta& operator+(const Meta&);
		Meta& operator-(const Meta&);
		Meta& operator*(const Meta&);
		Meta& operator/(const Meta&);
		Meta& operator+=(const Meta&);
		Meta& operator-=(const Meta&);
		Meta& operator*=(const Meta&);
		Meta& operator/=(const Meta&);

		double operator()(const std::map<Variable, double>&);
		double operator()();
		std::partial_ordering operator<=>(const Meta&) const;
		bool operator==(const Meta&) const;
		
		Meta& operator<<(Calculation&&);
		Meta& operator>>(const Meta&);
	};
}
#endif