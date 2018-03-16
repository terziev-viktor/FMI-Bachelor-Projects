#include <iostream>
#include <fstream>
#include <time.h>
#include <Windows.h>

#include "IdGenerator.h"
#include "Tools.h"

using std::cin;
using std::cout;
using std::endl;

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

bool AddWallet(double fiatMoney, char owner[256])
{
	std::ofstream file;
	file.open(WALLETS, std::ios::binary | std::ios::app);
	if (!file.good())
	{
		file.close();
		return false;
	}
	double coins;
	coins = fiatMoney / LEV_TO_FMICOINT_COURCE;
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

bool UpdateWallet(unsigned int id, Type updateType, double coins)
{
	std::fstream file;
	file.open(WALLETS, std::ios::binary);
	if (!file.good())
	{
		file.close();
		return false;
	}
	Wallet w;
	while (true)
	{
		file.read(reinterpret_cast<char*>(&w.id), sizeof(unsigned));
		if (id == w.id)
		{
			double money;
			double coinsToMoney = coins * LEV_TO_FMICOINT_COURCE; // conv his coins to real money
			file.read(reinterpret_cast<char*>(&money), sizeof(double));
			if (updateType == Type::SELL) // If he has sold some coins => he has earned money and vice versa
			{
				money += coinsToMoney;
			}
			else
			{
				money -= coinsToMoney;
			}
			file.seekp(-sizeof(double), std::ios::cur); // set the put pointer just before fiat money so we can overrite
			file.write(reinterpret_cast<char*>(&coins), sizeof(double));
			break;
		}
		file.seekg(sizeof(double) + sizeof(char) * 256, std::ios::cur);
		if (file.eof() || file.bad())
		{
			break;
		}
	}
	file.close();
	return true;
}

bool MakeOrder(char typeStr[], double coins, unsigned int id)
{
	Order order;
	order.fmiCoins = coins;
	order.walletId = id;
	bool convSuccess = CharArrayToType(typeStr, order.type);
	int orderSize = sizeof(char) * 256 + sizeof(double) + sizeof(unsigned);
	if (!convSuccess)
	{
		return false;
	}
	Wallet w;
	bool success = GetWalletById(order.walletId, w);
	bool feasible = IsFeasible(order, w);

	std::ifstream in;
	in.open(ORDERS, std::ios::binary);
	bool inSuccess = in.good();
	if (!inSuccess)
	{
		in.close();
		return false;
	}
	in.seekg(0, std::ios::ate);
	long sizeInBytes = in.tellg();
	long ordersCount = sizeInBytes / orderSize;
	Order * orders = new Order[ordersCount]; // get all orders from the file, update and write them back
	int * satisfiedOrders = new int[ordersCount]; // selling/buying may satisfy orders of other users
	int indexSatisfiedOrders = 0;
	for (int i = 0; i < ordersCount; i++)
	{
		in.read(reinterpret_cast<char*>(&orders[i].walletId), sizeof(unsigned));
		in.read(reinterpret_cast<char*>(&orders[i].type), sizeof(Type));
		in.read(reinterpret_cast<char*>(&orders[i].fmiCoins), sizeof(double));
	}
	in.close();
	if (order.type == Type::BUY)
	{
		bool fullySatisfied = false;
		for (int i = 0; i < ordersCount && !fullySatisfied; i++)
		{
			if (orders[i].type == Type::SELL)
			{
				int comparison = Compare(orders[i].fmiCoins, coins); // check if this old order has enough coins to sell
				double coinsToSell;
				if (comparison == -1) // if the current order is for more coins that this old order has to sell then sell as much as possible
				{
					coinsToSell = orders[i].fmiCoins;
				}
				else
				{
					coinsToSell = coins;
				}
				MakeTransaction(orders[i].walletId, order.walletId, coinsToSell);
				bool updateSuccess = UpdateWallet(orders[i].walletId, Type::SELL, coinsToSell);
				updateSuccess = UpdateWallet(order.walletId, Type::BUY, coinsToSell);
				coins -= coinsToSell; // update in case if not full satisfaction
				if (!updateSuccess)
				{
					// exception should be handled
				}

				if (comparison == 0 || comparison == 1)
				{
					fullySatisfied = true;
				}
			}
		}
		if (!fullySatisfied)
		{
			// write in file a the new order with coins that left

		}
	}

	delete[] orders;
	return true;
}

bool WalletInfo(unsigned int id)
{
	Wallet w;
	bool success = GetWalletById(id, w);
	if (!success)
	{
		return false;
	}
	success = CalcCoinsByTransactions(w.id, w);
	if (!success)
	{
		return false;
	}
	Print(w);
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
			double fiatmoney;
			char name[256];
			cin >> fiatmoney;
			cin.ignore();
			cin.getline(name, '\n');
			bool success = AddWallet(fiatmoney, name);
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
			char type[5];
			double coins;
			unsigned int id;
			cin >> type >> coins >> id;
			bool success = MakeOrder(type, coins, id);
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
			unsigned int id;
			cin >> id;
			bool success = WalletInfo(id);
			if (!success)
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
	srand((unsigned int)(time(NULL))); // set random seed for id generating function
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // console's handle - for changing text color

	Load();
	Run(hConsole);

	return 0;
}