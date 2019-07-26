#pragma once
#include "NAutomata.h"

#include <vector>
#include <memory>

namespace ak47
{
	class AutomataOperator
	{
	public:
		AutomataOperator(char c);

		virtual ~AutomataOperator();

		bool operator==(const AutomataOperator& b);
		bool operator==(char c);

		virtual bool operator()(const NAutomata& LeftArg, const NAutomata& RightArg, NAutomata & out) = 0;

	private:
		char symbol;
	};

	class AutomataOperators
	{
	public:
		static AutomataOperators& TheInstance();
		std::shared_ptr<AutomataOperator> Get(char c);
		bool IsValidOperator(char c);
		void RegisterOperator(std::shared_ptr<AutomataOperator> op);
		
		AutomataOperators(const AutomataOperators&&) = delete;
		AutomataOperators(const AutomataOperators&)  = delete;
		void operator=(const AutomataOperators&)	 = delete;
		void operator=(const AutomataOperators&&)	 = delete;
	private:
		AutomataOperators();

		std::vector<std::shared_ptr<AutomataOperator>> operators;
	};

}