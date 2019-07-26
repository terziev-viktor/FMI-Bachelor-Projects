#include "pch.h"
#include "Regex.h"
#include "AutomataOperator.h"
using ak47::Regex;
using ak47::NAutomata;
using std::string;
using ak47::AutomataOperators;
using ak47::AutomataOperator;

Regex::Regex(const std::string& regex)
{
	this->regex = regex;
	this->isInit = false;
}

ak47::Regex::Regex(const std::string& regex, bool Init)
{
	this->regex = regex;
	this->Init();
}

bool ak47::Regex::IsMatch(const std::string& str)
{
	return false;
}

bool _Init(string::iterator& ch, NAutomata * out)
{
	char c = *ch;
	
	std::shared_ptr<AutomataOperator> the_ptr = AutomataOperators::TheInstance().Get(c);
	
	return false;
}

void ak47::Regex::Init()
{
	string::iterator pos = this->regex.begin();
	NAutomata* out = &this->automata;
	if (!_Init(pos, out))
	{
		throw std::exception("Wrong regex expression");
	}
}
