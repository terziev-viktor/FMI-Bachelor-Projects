#include "Tools.h"

#include <iostream>
using std::cout;
using std::endl;

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
	cout << w.id << " " << w.fiatMoney << " " << w.owner << endl;
}

void Print(const Wallet & w, double fmiCoins)
{
	cout << w.id << " " << w.fiatMoney << " FMICoins: " << fmiCoins << " " << w.owner << endl;
}
