#ifndef BDUI_META
#define BDUI_META

#include <string>

namespace BdUI{
	namespace MetaType {
		enum MetaType
		{
			Variable = 0,
			Const = 1,
		};
	}

	template<MetaType::MetaType> class Meta;

	class Meta<MetaType::Const> {
	public:
		Meta(double n, double p = 1) : number(n),pow(p);
		double pow;
		double number;
	};

	class Meta<MetaType::Variable> {
	public:
		Meta<MetaType::Const> pow;
		char variable;
		Meta<MetaType::Const> coefficient;
		std::string subscript;
	};

	
}