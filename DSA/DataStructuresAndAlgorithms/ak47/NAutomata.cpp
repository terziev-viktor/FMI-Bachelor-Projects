#include "pch.h"

ak47::NAutomata::NAutomata(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals)
{
	this->Init(alphabet, nQ, start, Finals);
}

void ak47::NAutomata::Init(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals)
{
	this->alphabet = alphabet;
	this->SetStart(start);
	this->A.reserve(nQ);
	for (size_t i = 0; i < nQ; i++)
	{
		this->A.push_back(Q());
	}
	this->SetFinals(Finals);
}

void ak47::NAutomata::operator()(size_t from, char with, const vector<size_t>& to)
{
	this->fDelta(from, with, to, false);
}

void ak47::NAutomata::SetStart(size_t start)
{
	if (this->A.size() > start)
	{
		this->start = start;
	}
}

void ak47::NAutomata::SetFinals(const vector<size_t> Finals)
{
	size_t n = this->A.size();
	for (auto& i : A)
	{
		i.isFinal = false;
	}
	for (auto& i : Finals)
	{
		if (i < n)
		{
			A[i].isFinal = true;
		}
	}
}

bool ak47::NAutomata::Matches(const string& str) const
{
	if (str.empty() && A[start].isFinal)
	{
		return true;
	}
	else
	{
		vector<size_t> reachable;
		reachable.push_back(start);
		vector<size_t> next_reachable;

		size_t n = str.size();
		char with;
		for (size_t i = 0; i < n; ++i)
		{
			with = str[i];
			next_reachable.clear();
			for (auto& from : reachable)
			{
				auto iter = this->A[from].q.find(with);
				if (iter != this->A[from].q.end())
				{
					const vector<size_t>& tmp = iter->second;
					for (const auto& k : tmp)
					{
						next_reachable.push_back(k);
					}
				}
			}
			reachable.swap(next_reachable);
		}
		for (auto& i : reachable)
		{
			if (A[i].isFinal)
			{
				return true;
			}
		}
		return false;
	}
}


void ak47::NAutomata::fDelta(size_t from, char with, const vector<size_t>& t, bool force)
{
	if (!force)
	{
		if (alphabet.find(with) == string::npos)
		{
			throw std::exception("Not in the alpthabet");
		}
	}

	for (auto& to : t)
	{
		if (from < A.size() || to < A.size())
		{
			A[from].q[with].push_back(to);
		}
		else
		{
			throw std::exception("Invalid parameters 'from' and/or 'to'");
		}
	}
}

ak47::NAutomata ak47::operator+(const Automata& A, const Automata& B)
{
	string ab_alphabet = A.Alphabet();
	size_t n = B.Alphabet().size();
	for (size_t i = 0; i < n; i++)
	{
		if (ab_alphabet.find(B.Alphabet()[i]) == string::npos) // if there is a new letter that should be added
		{
			ab_alphabet += B.Alphabet()[i];
		}
	}
	vector<size_t> finals = B.GetFinals();
	for (size_t i = 0; i < finals.size(); i++)
	{
		finals[i] += A.nQ(); // Qs of B are numbered after those of A
	}
	if (B.IsFinal(B.GetStart())) // if B start is Final we can match every word that A mathes
	{
		vector<size_t> tmp = A.GetFinals();
		for (auto& i : tmp)
		{
			finals.push_back(i);
		}
	}
	NAutomata AB(ab_alphabet, A.nQ() + B.nQ(), A.GetStart(), finals);
	vector<size_t> tmp; // optimizing expand of fDelta
	// expanding fdelta with A
	for (size_t i = 0; i < A.Qs().size(); i++)
	{
		
		for (auto& q : A.Qs()[i].q)
		{
			tmp.clear();
			tmp.push_back(q.second);
			AB.fDelta(i, q.first, tmp, true);
		}
	}
	// Expanding fdelta with B
	for (size_t i = 0; i < B.Qs().size(); i++)
	{
		for (auto& q : B.Qs()[i].q)
		{
			tmp.clear();
			tmp.push_back(q.second + A.nQ()); // Qs of B are numbered after those of A
			AB.fDelta(i + A.nQ(), q.first, tmp, true);
		}
	}
	n = A.Qs().size();
	for (size_t i = 0; i < n; i++) // expanding the fDelta to link A with B
	{
		if (A.Qs()[i].isFinal) // for each final of A, imitate start of B
		{
			
			for (auto& j : B.Qs()[B.GetStart()].q) // start of B
			{
				tmp.clear();
				tmp.push_back(j.second + A.nQ());
				AB.fDelta(i, j.first, tmp, true);
			}
		}
	}
	return std::move(AB); // use move constructor
}
