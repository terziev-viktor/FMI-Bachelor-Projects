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
	// save the new wallet -> id, fiatmoney, owner
	file.write(reinterpret_cast<char*>(&id), sizeof(unsigned int));
	file.write(reinterpret_cast<char*>(&fiatMoney), sizeof(double));
	int sizeofowner = sizeof(owner);
	file.write(reinterpret_cast<char*>(&sizeofowner), sizeof(int));
	file.write(reinterpret_cast<char*>(&owner), sizeofowner);

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

int main()
{
	srand(time(NULL)); // set random seed for id generating function
	std::ifstream file;
	file.open(WALLETS, std::ios::binary);
	
	if (file.good())
	{
		while (!file.eof())
		{
			int id;
			file.read( reinterpret_cast<char*>(&id), sizeof(int));
			cout << id << endl;
			double fiatMoney;
			file.read( reinterpret_cast<char*>(&fiatMoney), sizeof(double));
			cout << fiatMoney << endl;
			int sizeofowner;
			file.read(reinterpret_cast<char*>(&sizeofowner), sizeof(int));
			cout << sizeofowner << endl;
			char owner[256];
			file.read(reinterpret_cast<char*>(&owner), sizeofowner);
			cout << owner << endl;
		}
	}

	file.close();

	char command[18];
	cin >> command;
	while (strcmp(command, COMMAND_QUIT) != 0)
	{
		if (strcmp(command, COMMAND_ADD_WALLET) == 0)
		{
			bool success = AddWallet();
			if (!success)
			{
				cout << "Adding wallet failed." << endl;
			}
		}
		if (strcmp(command, COMMAND_MAKE_ORDER) == 0)
		{
			bool success = MakeOrder();
			if (!success)
			{
				cout << "Making order failed" << endl;
			}
		}

	}


	return 0;
}