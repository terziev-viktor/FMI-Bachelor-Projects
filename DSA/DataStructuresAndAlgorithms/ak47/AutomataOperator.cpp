#include "pch.h"
#include "AutomataOperator.h"

using namespace ak47;

AutomataOperator::AutomataOperator(char c)
{
	this->symbol = c;
	AutomataOperators::TheInstance().RegisterOperator(std::move(std::shared_ptr<AutomataOperator>(this)));
}

AutomataOperator::~AutomataOperator()
{
}

bool AutomataOperator::operator==(const AutomataOperator& b)
{
	return this->symbol == b.symbol;
}

bool ak47::AutomataOperator::operator==(char c)
{
	return this->symbol == c;
}

AutomataOperators& ak47::AutomataOperators::TheInstance()
{
	static AutomataOperators the_instance;
	return the_instance;
}

ak47::AutomataOperators::AutomataOperators()
{

}

std::shared_ptr<AutomataOperator> ak47::AutomataOperators::Get(char c)
{
	for (auto& i : AutomataOperators::TheInstance().operators)
	{
		if (*i == c)
		{
			return i;
		}
	}
	throw std::exception("Non existing Automataoperator");
}

bool ak47::AutomataOperators::IsValidOperator(char c)
{
	for (auto& i : AutomataOperators::TheInstance().operators)
	{
		if (*i == c)
		{
			return true;
		}
	}
	return false;
}

void ak47::AutomataOperators::RegisterOperator(std::shared_ptr<AutomataOperator> op)
{
	this->operators.push_back(std::move(op));
}
