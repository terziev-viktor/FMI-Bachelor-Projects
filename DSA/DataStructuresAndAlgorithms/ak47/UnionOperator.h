#pragma once
#include "AutomataOperator.h"

namespace ak47
{
	class UnionOperator :
		public AutomataOperator
	{
	public:
		UnionOperator();

		virtual ~UnionOperator();

		bool operator()(const NAutomata& LeftArg, const NAutomata& RightArg, NAutomata& out) override;
	};
}



