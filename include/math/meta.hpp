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
#include <numbers>
#include <array>
#include <any>
#include <deque>
#include <codecvt>

namespace BdUI{
	enum class Calculation {
		plus,		// 加法运算
		minus,		// 减法运算
		multi,		// 乘法运算
		divide,		// 除法运算
		pow,		// 幂运算
		log,		// 对数运算
		derivate,	// 求导运算
		sum,		// 求和运算
		fac,		// 阶乘运算
	};

	class Variable : public std::pair < Character,
		std::variant<std::monostate,
		std::vector<int>,
		std::vector<Character>>> {
	public:
		static const std::variant<std::monostate,
			std::vector<int>,
			std::vector<Character>> no_subscript;
		bool changable_subscript = true;

		Variable(const Character&, const std::string&);
		Variable(const Character&, const std::initializer_list<char>&);
		Variable(const Character& c, const std::initializer_list<int>& init) : std::pair < Character,
			std::variant<std::monostate,
			std::vector<int>,
			std::vector<Character>>>{c,init} {}

		Variable(char c) : std::pair < Character,
			std::variant<std::monostate,
			std::vector<int>,
			std::vector<Character>>>{c, no_subscript} {}
		Variable(const Character& c) : std::pair < Character,
			std::variant<std::monostate,
			std::vector<int>,
			std::vector<Character>>>{ c, no_subscript } {}

		using std::pair < Character,
			std::variant<std::monostate,
			std::vector<int>,
			std::vector<Character>>>::pair;
		using std::pair < Character,
			std::variant<std::monostate,
			std::vector<int>,
			std::vector<Character>>>::operator=;

		bool operator==(const Variable&) const = default;
		bool operator<(const Variable&) const;
	};
	inline constexpr std::variant<std::monostate,
		std::vector<int>,
		std::vector<Character>> Variable::no_subscript;

	class Meta {
	private:
		std::vector<Variable> all_variables;
		std::variant<double,size_t> coefficient;
		std::optional<Calculation> operator_calculation;
		std::list<std::pair<Calculation, Meta>> Content;
		std::deque<std::pair<Calculation, std::any>> operation;

		void Add(const Meta&);
		void Minus(const Meta&);
		static double _calculate(const Meta&,std::deque<std::pair<Calculation, std::any>>&,std::map<Variable, double>&);
	public:
		const std::vector<Variable>& Allvariables = all_variables;
		const std::list<std::pair<Calculation, Meta>>& content = Content;

		Meta(const double& d) : coefficient(d) {}
		Meta(const int& i) : coefficient((double)i) {}
		Meta(const char& c) : coefficient(size_t(0)), all_variables{ c } {}
		Meta(const Variable& v) : coefficient(size_t(0)), all_variables{ v } {}
		Meta(const Meta&) = default;
		Meta(const std::string& LATEX_expression);

		double calculate(const std::map<Variable,double>& = std::map<Variable,double>()) const;

		Meta& erase(const std::list<std::pair<Calculation, Meta>>::const_iterator&);
		Meta& emplace(const std::list<std::pair<Calculation, Meta>>::const_iterator&, const std::pair<Calculation, Meta>&);
		Meta& insert(const std::list<std::pair<Calculation, Meta>>::const_iterator&, const std::pair<Calculation, Meta>&);
		Meta& pow(const Meta&);
		Meta& log(const Meta&);
		Meta& plus(const Meta&);
		Meta& minus(const Meta&);
		Meta& multi(const Meta&);
		Meta& divide(const Meta&);
		Meta& sum(const std::array<Character, 2>&);
		Meta& fac();

		Meta& operator+(const Meta&);
		Meta& operator-(const Meta&);
		Meta& operator*(const Meta&);
		Meta& operator/(const Meta&);
		Meta& operator+=(const Meta&);
		Meta& operator-=(const Meta&);
		Meta& operator*=(const Meta&);
		Meta& operator/=(const Meta&);

		double operator()(const std::map<Variable, double>& = std::map<Variable,double>());
		std::partial_ordering operator<=>(const Meta&) const;
		bool operator==(const Meta&) const;

		Meta& operator=(const Meta&);
		
		Meta& operator<<(Calculation&&);
		Meta& operator>>(const Meta&);
		Meta& operator>>(const std::array<Character, 2>&);
	};
}
#endif