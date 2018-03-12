#include <iostream>
#include <fstream>
#include <time.h>
#include <Windows.h>

#include "IdGenerator.h"
#include "Tools.h"

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
const char COMMAND_LIST_WALLETS[] = "list-wallets";

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
	char typeStr[5];
	cin >> typeStr;
	Type type;
	bool success = CharArrayToType(typeStr, type);
	if (!success)
	{
		file.close();
		return false;
	}
	double coins;
	cin >> coins;
	if (coins < 0)
	{
		file.close();
		return false;
	}
	unsigned int walletId;
	cin >> walletId;

	file.write(reinterpret_cast<char*>(&walletId), sizeof(unsigned int));
	file.write(reinterpret_cast<char*>(&type), sizeof(Type::SELL));
	file.write(reinterpret_cast<char*>(&coins), sizeof(double));

	file.close();
	return true;
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
	// TODO: .eof() is not being triggered for some reason
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
			file.seekg(sizeof(double));
		}
	}
	out = coins;
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

	Wallet w;
	while (!file.eof())
	{
		file.read(reinterpret_cast<char*>(&w.id), sizeof(unsigned int));
		if (w.id == id)
		{
			file.read(reinterpret_cast<char*>(&w.fiatMoney), sizeof(double));
			file.read(reinterpret_cast<char*>(&w.owner), sizeof(char) * 256);
			file.close();
			double coins;
			CalcCoinsByTransactions(w.id, coins);
			Print(w, coins);
			file.close();
			return true;
		}
		else
		{
			file.seekg(sizeof(double) + (sizeof(char) * 256), std::ios::cur);
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

bool ListWallets()
{
	int n;
	cin >> n;
	std::ifstream file;
	file.open(WALLETS, std::ios::binary);
	if (!file.good())
	{
		file.close();
		return 0;
	}
	unsigned int id;
	double fiatmoney;
	char name[256];
	for (int i = 0; i < n; i++)
	{
		file.read(reinterpret_cast<char*>(&id), sizeof(unsigned int));
		file.read(reinterpret_cast<char*>(&fiatmoney), sizeof(double));
		file.read(reinterpret_cast<char*>(&name), sizeof(char) * 256);
		cout << i << ". " << id << " " << fiatmoney << " " << name << endl;
		if (file.eof())
		{
			break;
		}
	}
	file.close();
	return true;
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
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << "Viewing wallet info failed" << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			continue;
		}

		if (strcmp(command, COMMAND_ATTRACT_INVESTORS) == 0)
		{
			bool success = AttractInvestors();
			if (!success)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << "Attracting investors failed" << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			continue;
		}

		if (strcmp(command, COMMAND_LIST_WALLETS) == 0)
		{
			bool success = ListWallets();
			if (!success)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << "Failed listing wallets";
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
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
		cout << "Wallets:" << endl;
		unsigned int id;
		double money;
		char name[256];
		while (true)
		{
			file.read(reinterpret_cast<char*>(&id), sizeof(unsigned int));
			file.read(reinterpret_cast<char*>(&money), sizeof(double));
			file.read(reinterpret_cast<char*>(&name), sizeof(char) * 256);
			if (!file.eof())
			{
				cout << id << " " << money << " " << name << endl;
			}
			else
			{
				break;
			}
		}
	}
	file.close();

	std::ifstream transactions;
	transactions.open(TRANSACTIONS, std::ios::binary);
	cout << endl;
	if (transactions.good())
	{
		cout << "Transactions" << endl;
		unsigned sender;
		unsigned reciever;
		double coins;
		while (true)
		{
			transactions.read(reinterpret_cast<char*>(&sender), sizeof(sender));
			transactions.read(reinterpret_cast<char*>(&reciever), sizeof(reciever));
			transactions.read(reinterpret_cast<char*>(&coins), sizeof(double));
			if (!transactions.eof())
			{
				cout << sender << "->" << reciever << " " << coins << endl;
			}
			else
			{
				break;
			}
		}
	}
	file.close();

	std::ifstream orders;
	orders.open(ORDERS, std::ios::binary);
	if (orders.good())
	{
		Type type;
		double coins;
		unsigned int walletId;
		cout << "Orders:" << endl;
		while (true)
		{
			orders.read(reinterpret_cast<char*>(&walletId), sizeof(unsigned));
			orders.read(reinterpret_cast<char*>(&type), sizeof(Type::SELL));
			orders.read(reinterpret_cast<char*>(&coins), sizeof(double));
			if (!orders.eof())
			{
				cout << walletId << " - " << type << " - " << coins << endl;
			}
			else
			{
				break;
			}
		}
	}
	orders.close();
}

int main()
{
	srand((unsigned int) (time(NULL))); // set random seed for id generating function
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // console's handle - for changing text color

	Load();

	Run(hConsole);

	return 0;
}