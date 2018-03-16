#include "Tools.h"

#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
const double eps = 0.0000000001;

bool CharArrayToType(const char type[], Type & out)
{
	if (strcmp(SELL_AS_STR, type) == 0)
	{
		out = Type::SELL;
		return true;
	}
	else if(strcmp(BUY_AS_STR, type))
	{
		out = Type::BUY;
		return true;
	}
	return false;
}

void Print(const Wallet & w)
{
	cout << w.id << endl << w.fiatMoney << " -> " << w.fmiCoins << endl << w.owner << endl;
}

// return 1 if a > b; return -1 if b > a and return 0 if a == b;
int Compare(double a, double b)
{
	double c = a - b;
	if (c < eps)
	{
		return 0;
	}
	return c > 0 ? 1 : -1;
}


bool IsFeasible(Order ord, Wallet w)
{
	if (ord.type == Type::BUY)
	{
		double coinsToMoney = ord.fmiCoins * LEV_TO_FMICOINT_COURCE;
		int comparison = Compare(coinsToMoney, w.fiatMoney);
		bool hasTheMoney = comparison == -1 || comparison == 0;
		return hasTheMoney;
	}
	else if (ord.type == Type::SELL)
	{
		bool hasCoins = w.
	}

}

bool GetWalletById(unsigned id, Wallet w)
{
	std::ifstream in;
	in.open(WALLETS, std::ios::binary);
	if (!in.good())
	{
		in.close();
		return false;
	}
	while (true)
	{
		in.read(reinterpret_cast<char*>(&w.id), sizeof(unsigned));
		if (in.eof())
		{
			return false;
		}
		if (w.id == id)
		{
			in.read(reinterpret_cast<char*>(&w.fiatMoney), sizeof(double));
			in.read(reinterpret_cast<char*>(&w.owner), sizeof(char) * 256);
			return true;
		}
		else
		{
			in.seekg(sizeof(double), std::ios::cur);
			in.seekg(sizeof(char) * 256, std::ios::cur);
			continue;
		}
	}
}

bool CalcCoinsByTransactions(unsigned int id, double & out)
{
	std::ifstream file;
	file.open(TRANSACTIONS, std::ios::binary);
	if (!file.good())
	{
		file.close();
		return false;
	}
	double coins = 0, c;
	unsigned int sender, reciever;
	int index = 0;
	while (true)
	{
		file.read(reinterpret_cast<char*>(&sender), sizeof(unsigned));
		file.read(reinterpret_cast<char*>(&reciever), sizeof(unsigned));
		if (file.eof())
		{
			break;
		}
		if (sender == id)
		{
			file.read(reinterpret_cast<char*>(&c), sizeof(double));
			coins -= c;
			continue;
		}
		else if (reciever == id)
		{
			file.read(reinterpret_cast<char*>(&c), sizeof(double));
			coins += c;
			continue;
		}
		else
		{
			file.seekg(sizeof(double), std::ios::cur);
		}
	}
	out = coins;
	return true;
}