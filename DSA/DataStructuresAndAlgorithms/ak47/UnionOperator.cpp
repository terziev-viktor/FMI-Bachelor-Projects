#include "pch.h"
#include "UnionOperator.h"

ak47::UnionOperator::UnionOperator()
	:AutomataOperator('+')
{
	
}

ak47::UnionOperator::~UnionOperator()
{
}

bool ak47::UnionOperator::operator()(const NAutomata& LeftArg, const NAutomata& RightArg, NAutomata& out)
{
	out = LeftArg;
	out += RightArg;
	return true;
}

static ak47::UnionOperator the_operator;