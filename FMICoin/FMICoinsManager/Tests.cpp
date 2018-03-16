#include "Tests.h"

bool Test_GetWalletById()
{
	unsigned int testId = 1;
	Wallet w;
	Wallet expeced;
	expeced.id = 1;
	strcpy(expeced.owner, "Viktor A Terziev");
	expeced.fiatMoney = 100;
	bool success = GetWalletById(testId, w);
	if (!success)
	{
		return false;
	}
	bool idCorrect = expeced.id == w.id;
	bool ownerCorrect = strcmp(expeced.owner, w.owner) == 0;
	bool moneyCorrect = Compare(expeced.fiatMoney, w.fiatMoney) == 0;
	return idCorrect && ownerCorrect && moneyCorrect;
}

bool RunAllTests()
{
	bool test1 = Test_GetWalletById();
	return test1;
}