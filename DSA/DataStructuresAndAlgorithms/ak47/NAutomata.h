#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "Automata.h"

namespace ak47
{
	using std::vector;
	using std::unordered_map;
	using std::string;

	// Non deterministic Automata
	class NAutomata
	{
	public:
		friend NAutomata operator+(const Automata& A, const Automata& B);

		struct Q
		{
			unordered_map<char, vector<size_t>> q;
			bool isFinal;
			Q() :isFinal(false) {}
		};

		NAutomata()
			:start(0), isInit(false)
		{	}

		NAutomata(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals);

		void Init(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals);

		void operator()(size_t from, char with, const vector<size_t> & to);

		void SetStart(size_t start);

		inline size_t GetStart() const { return this->start; }

		void SetFinals(const vector<size_t> Finals);

		inline const string& Alphabet() const { return this->alphabet; }

		bool Matches(const string& str) const;

		inline const vector<NAutomata::Q>& Qs() const { return this->A; }
	protected:
		vector<Q> A;
		size_t start;
		string alphabet;
		bool isInit;
		void fDelta(size_t from, char with, const vector<size_t> & to, bool force);
	};
	
	NAutomata operator+(const Automata& A, const Automata& B);

}
