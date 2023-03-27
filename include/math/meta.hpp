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
#include <any>
#include <deque>
#include <codecvt>
#include <stack>
#include <regex>
#include <set>
#include <memory>
#include <forward_list>

namespace BdUI{
	class Variable : public std::pair < Character,
		std::vector<std::variant<int, std::string>>> {
	private:
		static const std::regex variable_regex;
	public:
		static const std::vector<std::variant<int, std::string>> no_subscript;
		bool changable_subscript = true;

		Variable(const char c) : std::pair<Character,std::vector<std::variant<int, std::string>>>{ c,no_subscript } {}
		Variable(const Character& c, const std::vector<std::variant<int,std::string>>& init = std::vector<std::variant<int, std::string>>()) : std::pair < Character,
			std::vector<std::variant<int, std::string>>>{ c,init } {}
		Variable(const std::string& Latex);

		static std::vector<Variable> GetVariables(const std::string& Latex);

		using std::pair < Character,
			std::vector<std::variant<int, std::string>>>::pair;
		using std::pair < Character,
			std::vector<std::variant<int, std::string>>>::operator=;

		bool operator==(const Variable&) const = default;
		bool operator<(const Variable&) const = default;
	};

	class Meta {
	private:
		enum class Calculation {
			plus,		// 加法运算
			minus,		// 减法运算
			multi,		// 乘法运算
			divide,		// 除法运算

			pow,		// 幂运算
			log,		// 对数运算

			fac,		// 阶乘运算
			cos,		// 余弦运算
			sin,		// 正弦运算

			derivate,	// 求导运算
			intergeral,	// 积分运算
			sum,		// 求和运算
			multiply,	// 求乘积运算

			left_bracket,	//左括号
			right_bracket,	//右括号
		};

		static long double _calculate(std::stack<std::variant<Meta, Calculation>>&, std::map<Variable, long double>&);
		static long double _calculate(Meta&, std::map<Variable, long double>&);
		static Meta StrToVariables(const std::string&);

		static const std::map<std::string, Calculation> function_name;
		static const std::map<Calculation, std::regex> function_regex;
		static const std::map<Calculation, unsigned char> operation_parameter_number;
		static const std::map<Calculation, unsigned char> opera;
		static const std::map<char, Calculation> opera_calculation;

		static const std::regex function_name_regex;
		static const std::regex vari_with_num_regex;
		
		std::variant<long double, std::unique_ptr<Variable>,
			std::unique_ptr<std::pair<std::vector<Variable>,std::stack<std::variant<Meta,Calculation>>>>> content;

		void CalculationAdd(Calculation, const std::vector<Meta>& = std::vector<Meta>());
	public:
		Meta(const long double& d) : content(d) {}
		Meta(const int& i) : content((long double)i) {}
		Meta(const char& c) : content(std::make_unique<Variable>(c)) {}
		Meta(const Variable& v) :content(std::make_unique<Variable>(v)) {}
		Meta(const Meta&);
		Meta(const std::string& LATEX_expression);
		Meta(const char*);

		long double calculate(const std::map<Variable,long double>& = std::map<Variable,long double>()) const;

		Meta& pow(const Meta&);
		Meta& minus(const Meta&);
		Meta& multi(const Meta&);
		Meta& divide(const Meta&);

		Meta& log(const Meta&);
		Meta& plus(const Meta&);

		Meta& fac();
		Meta& sin();
		Meta& cos();

		Meta& sum(const std::pair<Meta, Meta>&);
		

		Meta& operator+(const Meta&);
		Meta& operator-(const Meta&);
		Meta& operator*(const Meta&);
		Meta& operator/(const Meta&);
		Meta& operator+=(const Meta&);
		Meta& operator-=(const Meta&);
		Meta& operator*=(const Meta&);
		Meta& operator/=(const Meta&);

		Meta& operator=(const Meta&);

		long double operator()(const std::map<Variable, long double>& = std::map<Variable,long double>());

		std::partial_ordering operator<=>(const Meta&) const = default;
		bool operator==(const Meta&) const = default;
	};
}
#endif