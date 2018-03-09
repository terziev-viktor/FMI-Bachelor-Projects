#include <iostream>
#include <fstream>
#include <ctime>
#include <Windows.h>

#include "IdGenerator.h"
#include "Models.h"
using std::cin;
using std::cout;
using std::endl;
// Consts
const int CONSOLE_GREEN = 10;
const int CONSOLE_DEFAULT = 7;
const int CONSOLE_RED = 12;
const int LEV_TO_FMICOINT_COURCE = 375;
const unsigned int SYSTEM_WALLET = 4294967295;
// Files
const char WALLETS[] = "wallets.dat";
const char ORDERS[] = "orders.dat";
const char TRANSACTIONS[] = "transactions.dat";
// Commands
const char COMMAND_QUIT[] = "quit";
const char COMMAND_ADD_WALLET[] = "add-wallet";
const char COMMAND_MAKE_ORDER[] = "make-order";
const char COMMAND_WALLET_INFO[] = "wallet-info";
const char COMMAND_ATTRACT_INVESTORS[] = "attract-investors";

void print(const Wallet & w)
{
	cout << w.id << " " << w.fiatMoney << " " << w.owner << endl;
}

bool MakeTransaction(unsigned int senderId, unsigned int recieverId, double coins)
{
	std::ofstream file;
	file.open(TRANSACTIONS, std::ios::binary | std::ios::app);
	if (!file.good())
	{
		file.close();
		return false;
	}
	file.write(reinterpret_cast<char*>(&senderId), sizeof(unsigned int));
	file.write(reinterpret_cast<char*>(&recieverId), sizeof(unsigned int));
	file.write(reinterpret_cast<char*>(&coins), sizeof(double));

	file.close();
	return true;
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

	double fiatMoney, coins;
	cin >> fiatMoney;
	coins = fiatMoney / LEV_TO_FMICOINT_COURCE;

	char owner[256];
	cin.ignore();
	cin.getline(owner, 256);

	unsigned int id = GenerateId();
	
	// write to wallets.dat 
	// id -> fiatmoney -> owner [unsigned int][double][256 bytes]
	file.write(reinterpret_cast<char*>(&id), sizeof(unsigned int));
	file.write(reinterpret_cast<char*>(&fiatMoney), sizeof(double));
	file.write(reinterpret_cast<char*>(&owner), sizeof(char) * 256);
	file.close();

	// Put coins in the new wallet
	MakeTransaction(SYSTEM_WALLET, id, coins);

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

void Run(HANDLE &hConsole)
{
	char command[18];
	do
	{
		cin >> command;

		if (strcmp(command, COMMAND_ADD_WALLET) == 0)
		{
			bool success = AddWallet();
			if (success)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_GREEN);
				cout << "Wallet Added." << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			else
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << "Adding wallet failed." << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			continue;
		}

		if (strcmp(command, COMMAND_MAKE_ORDER) == 0)
		{
			bool success = MakeOrder();
			if (success)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_GREEN);
				cout << "New order created." << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			else
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << "Making order failed" << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
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
}

void Load()
{
	std::ifstream file;
	file.open(WALLETS, std::ios::binary);

	if (file.good())
	{
		unsigned int id;
		double money;
		char name[256];
		while (!file.eof())
		{
			file.read(reinterpret_cast<char*>(&id), sizeof(unsigned int));
			file.read(reinterpret_cast<char*>(&money), sizeof(double));
			file.read(reinterpret_cast<char*>(&name), sizeof(char) * 256);
			if (!file.eof())
			{
				cout << id << " " << money << " " << name << endl;
			}
		}
	}
	file.close();

	std::ifstream transactions;
	transactions.open(TRANSACTIONS, std::ios::binary);
	cout << endl;
	if (transactions.good())
	{
		unsigned sender;
		unsigned reciever;
		double coins;
		while (!transactions.eof())
		{
			transactions.read(reinterpret_cast<char*>(&sender), sizeof(sender));
			transactions.read(reinterpret_cast<char*>(&reciever), sizeof(reciever));
			transactions.read(reinterpret_cast<char*>(&coins), sizeof(double));
			if (!transactions.eof())
			{
				cout << sender << "->" << reciever << " " << coins << endl;
			}
		}
	}
	file.close();
}

int main()
{
	srand(time(NULL)); // set random seed for id generating function
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	Load();

	Run(hConsole);

	return 0;
}