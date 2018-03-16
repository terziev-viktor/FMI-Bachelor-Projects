#include "Models.h"
#include "Constants.h"

const char SELL_AS_STR[] = "SELL";
const char BUY_AS_STR[] = "BUY";

bool CharArrayToType(const char type[], Type & out);

void Print(const Wallet & w);

bool Compare(double, double);

bool IsFeasible(Order, Wallet);

bool GetWalletById(unsigned, Wallet w);

bool CalcCoinsByTransactions(unsigned int id, double & out);
