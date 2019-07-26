#include "pch.h"
#include "ConcatinationOperator.h"

ak47::ConcatinationOperator::ConcatinationOperator()
	:AutomataOperator('\0')
{

}

ak47::ConcatinationOperator::~ConcatinationOperator()
{
}

bool ak47::ConcatinationOperator::operator()(const NAutomata& LeftArg, const NAutomata& RightArg, NAutomata& out)
{
	return false;
}

static ak47::ConcatinationOperator the_operator;
