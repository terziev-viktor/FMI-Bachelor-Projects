#include "Models.h"

const char SELL_AS_STR[] = "SELL";
const char BUY_AS_STR[] = "BUY";

bool CharArrayToType(const char type[], Type & out);

void Print(const Wallet & w);

void Print(const Wallet & w, double fmiCoins);
