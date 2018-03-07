#include <iostream>
#include <fstream>
#include <time.h>

#include "IdGenerator.h"
#include "Models.h"
using std::cin;
using std::cout;
using std::endl;

const int LEV_TO_FMICOINT_COURCE = 375;

const char WALLETS[] = "wallets.dat";
const char ORDERS[] = "orders.dat";

const char COMMAND_QUIT[] = "quit";
const char COMMAND_ADD_WALLET[] = "add-wallet";
const char COMMAND_MAKE_ORDER[] = "make-order";
const char COMMAND_WALLET_INFO[] = "wallet-info";
const char COMMAND_ATTRACT_INVESTORS[] = "attract-investors";

void print(const Wallet & w)
{
	cout << w.id << " " << w.fiatMoney << " " << w.owner << endl;
}

bool AddWallet()
{
	std::ofstream file;
	file.open(WALLETS, std::ios::binary | std::ios::app);
	if (!file.good())
	{
		file.close();
		return false;
	}

	double fiatMoney;
	cin >> fiatMoney;

	char owner[256];
	cin.ignore();
	cin.getline(owner, 256);

	unsigned int id = GenerateId();
	Wallet w;
	w.id = id;
	w.fiatMoney = fiatMoney;
	strcpy_s(w.owner, owner);
	file.write(reinterpret_cast<char*>(&w), sizeof(Wallet));
	file.close();
	return true;
}

bool MakeOrder()
{
	std::ofstream file;
	file.open(ORDERS, std::ios::binary | std::ios::app);
	if (!file.good())
	{
		file.close();
		return false;
	}
	char type[4];
	cin >> type;
	double coins;
	cin >> coins;
	if (coins < 0)
	{
		file.close();
		return false;
	}
	unsigned int walletId;
	cin >> walletId;

	// TODO
	// Покупка:
	// Провери дали портфейла има достатъчно пари за монетите които ще купува
	// Удовлетвори нареждането за покупка
	// Ако не може да бъде удовлетворено напълно го запази в orders.dat
	// Създай съответните транзакции
	//
	// Продажба:
	// Провери дали в портвейла има достатъчно монети за продажба
	// Продай ги
	// Ако не могат да бъдат продадени ги пази в orders.dat
	// Създай съответните транзакции
	
	int sizeoftype = sizeof(type);
	file.write(reinterpret_cast<char*>(&sizeoftype), sizeof(int));
	file.write(reinterpret_cast<char*>(&type), sizeoftype);
	file.write(reinterpret_cast<char*>(&coins), sizeof(double));
	file.write(reinterpret_cast<char*>(&walletId), sizeof(unsigned int));

	file.close();
	return true;
}

bool WalletInfo()
{
	unsigned int id;
	cin >> id;
	std::ifstream file;
	file.open(WALLETS, std::ios::binary);
	if (!file.good())
	{
		file.close();
		return false;
	}

	while (file.eof())
	{
		Wallet w;
		file.read(reinterpret_cast<char*>(&w), sizeof(Wallet));
		if (w.id == id)
		{
			print(w);
			file.close();
			return true;
		}
	}
	cout << "Wallet doesn't exist." << endl;
	file.close();
	return true;
}

bool AttractInvestors()
{
	// TODO:
	return false;
}

int main()
{
	srand(time(NULL)); // set random seed for id generating function
	std::ifstream file;
	file.open(WALLETS, std::ios::binary);

	if (file.good())
	{
		while (!file.eof())
		{
			Wallet w;
			file.read(reinterpret_cast<char*>(&w), sizeof(Wallet));
			if (file.eof())
			{
				break;
			}
			print(w);
		}
	}

	file.close();

	char command[18];
	do
	{
		cin >> command;

		if (strcmp(command, COMMAND_ADD_WALLET) == 0)
		{
			bool success = AddWallet();
			if (success)
			{
				cout << "Wallet Added." << endl;
			}
			else
			{
				cout << "Adding wallet failed." << endl;
			}
			continue;
		}

		if (strcmp(command, COMMAND_MAKE_ORDER) == 0)
		{
			bool success = MakeOrder();
			if (success)
			{
				cout << "New order created." << endl;
			}
			else
			{
				cout << "Making order failed" << endl;
			}
			continue;
		}

		if (strcmp(command, COMMAND_WALLET_INFO) == 0)
		{
			bool success = WalletInfo();
			if (success)
			{
				cout << "Wallet:" << endl;
			}
			else
			{
				cout << "Viewing wallet info failed" << endl;
			}
			continue;
		}
		
		if (strcmp(command, COMMAND_ATTRACT_INVESTORS) == 0)
		{
			bool success = AttractInvestors();
			if (!success)
			{
				cout << "Attracting investors failed" << endl;
			}
			continue;
		}

	} while (strcmp(command, COMMAND_QUIT) != 0);

	return 0;
}