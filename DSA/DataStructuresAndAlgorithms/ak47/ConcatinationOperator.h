#pragma once
#include "AutomataOperator.h"

namespace ak47
{
	class ConcatinationOperator
		:public AutomataOperator
	{
	public:
		ConcatinationOperator();

		virtual ~ConcatinationOperator();

		bool operator()(const NAutomata& LeftArg, const NAutomata& RightArg, NAutomata& out) override;
	};

}