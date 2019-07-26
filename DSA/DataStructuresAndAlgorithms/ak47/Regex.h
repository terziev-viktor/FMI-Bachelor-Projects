#pragma once
#include "NAutomata.h"

namespace ak47
{
	using std::string;

	class Regex
	{
	public:
		Regex(const std::string& regex);
		Regex(const std::string& regex, bool Init);

		bool IsMatch(const std::string& str);

		void Init();

	protected:
		std::string regex;
		NAutomata automata;
		bool isInit;
	};

}