#include <iostream>
#include <fstream>
#include <time.h>
#include <Windows.h>

#include "ToolsH.h"
using std::cin;
using std::cout;
using std::endl;

bool AddTransaction(TransactionsContainer & transactions, Transaction & t)
{
	if (transactions.index >= transactions.size)
	{
		ExpandArr(transactions);
	}
	transactions.arr[transactions.index].senderId = t.senderId;
	transactions.arr[transactions.index].receiverId = t.receiverId;
	transactions.arr[transactions.index].fmiCoins = t.fmiCoins;
	transactions.arr[transactions.index].time = t.time;
	transactions.index++;

	return true;
}

bool AddWallet(double fiatMoney, char owner[256], WalletsContainer & wallets, char errmsg[100])
{
	if (wallets.index >= wallets.size)
	{
		ExpandArr(wallets);
	}
	wallets.arr[wallets.index].fiatMoney = fiatMoney;
	strcpy_s(wallets.arr[wallets.index].owner, sizeof(char)*256, owner);
	bool generated = GenerateId(wallets.arr[wallets.index].id);
	wallets.index++;
	if (!generated)
	{
		strcpy_s(errmsg, sizeof(char) * 100, "Cound not generate id.");
		return false;
	}
	return true;
}

bool AddOrder(Order & o, OrdersContainer & orders, TransactionsContainer & transactions, WalletsContainer & wallets, char errmsg[100])
{
	o.satisfied = false;
	int c = 0;
	double m = 0;
	for (long long i = 0; i < orders.index && !o.satisfied; i++)
	{
		if (o.type != orders.arr[i].type)
		{
			if (o.type == Type::BUY)	 // => orders.arr[i].type == SELL
			{
				int comparison = Compare(o.fmiCoins, orders.arr[i].fmiCoins);
				Transaction t;
				if (comparison == 1)
				{
					t.time = time(NULL);
					t.fmiCoins = orders.arr[i].fmiCoins;
					t.receiverId = o.walletId;
					t.senderId = orders.arr[i].walletId;
					o.fmiCoins -= orders.arr[i].fmiCoins;
					orders.arr[i].satisfied = true;
				}
				else if (comparison == 0)
				{
					t.time = time(NULL);
					t.senderId = orders.arr[i].walletId;
					t.receiverId = o.walletId;
					t.fmiCoins = o.fmiCoins;
					orders.arr[i].satisfied = true;
					o.satisfied = true;
				}
				else
				{
					t.time = time(NULL);
					t.fmiCoins = o.fmiCoins;
					t.senderId = orders.arr[i].walletId;
					t.receiverId = o.walletId;
					o.satisfied = true;
					orders.arr[i].fmiCoins -= o.fmiCoins;
				}
				if (!AddTransaction(transactions, t))
				{
					strcpy_s(errmsg, sizeof(char) * 100, "Transaction failed");
					return false;
				}
				if (!WriteTransaction(transactions, wallets, t, o, errmsg));
				{
					return false;
				}
				if (!UpdateWallet(wallets, transactions, t, errmsg))
				{
					return false;
				}
				m += t.fmiCoins;
			}
			else // o.type = SELL => orders.arr[i].type == BUY
			{
				int comparison = Compare(o.fmiCoins, orders.arr[i].fmiCoins);
				Transaction t;
				if (comparison == 1)
				{
					t.time = time(NULL);
					t.senderId = o.walletId;
					t.receiverId = orders.arr[i].walletId;
					t.fmiCoins = orders.arr[i].fmiCoins;
					
					orders.arr[i].satisfied = true;
					o.fmiCoins -= orders.arr[i].fmiCoins;
				}
				else if (comparison == 0)
				{
					t.time = time(NULL);
					t.senderId = o.walletId;
					t.receiverId = orders.arr[i].walletId;
					t.fmiCoins = o.fmiCoins;
					if (!AddTransaction(transactions, t))
					{
						strcpy_s(errmsg, sizeof(char) * 100, "Transaction failed");
						return false;
					}
					o.satisfied = true;
					orders.arr[i].satisfied = true;
				}
				else
				{
					t.time = time(NULL);
					t.senderId = o.walletId;
					t.receiverId = orders.arr[i].walletId;
					t.fmiCoins = o.fmiCoins;
					o.satisfied = true;
					orders.arr[i].fmiCoins -= o.fmiCoins;
				}
				if (!AddTransaction(transactions, t))
				{
					strcpy_s(errmsg, sizeof(char) * 100, "Transaction failed");
					return false;
				}
				if (!WriteTransaction(transactions, wallets, t, o, errmsg))
				{
					return false;
				}
				if (!UpdateWallet(wallets, transactions, t, errmsg))
				{
					return false;
				}
				m += t.fmiCoins;
			}
			c++;
		}
	}
	if (!WriteTransactionMeta(c, m, o, errmsg))
	{
		cout << errmsg << endl;
	}
	if (!o.satisfied)
	{
		if (orders.index >= orders.size)
		{
			ExpandArr(orders);
		}
		Cpy(orders.arr[orders.index], o);
		orders.index++;
	}
	return true;
}

bool WalletInfo(unsigned int id, WalletsContainer & wallets, TransactionsContainer & t, char errmsg[100])
{
	Wallet w;
	bool success = TellWalletById(id, wallets, w, t, errmsg);
	if (!success)
	{
		return false;
	}
	Print(w, false);
	return true;
}

bool AttractInvestors(WalletsContainer & wallets, TransactionsContainer & t, char errmsg[100])
{
	int size = 10;
	Wallet * top10 = new Wallet[size];
	for (int i = 0; i < size; i++)
	{
		top10[i].fiatMoney = 0;
		top10[i].fmiCoins = 0;
		strcpy_s(top10[i].owner, sizeof(char) * 256, " ");
	}
	
	for (int i = 0; i < wallets.index; i++)
	{
		if (!wallets.arr[i].fmiCoinsCalculated)
		{
			CalcFmiCoins(wallets.arr[i], t);
		}
		int j = 9;
		int comparrison = Compare(wallets.arr[i].fmiCoins, top10[j].fmiCoins);
		while (comparrison == 1 && j >= 0)
		{
			if (j < 9)
			{
				Cpy(top10[j + 1], top10[j]);
			}
			Cpy(top10[j], wallets.arr[i]);
			j--;
			comparrison = Compare(wallets.arr[i].fmiCoins, top10[j].fmiCoins);
		}
	}
	Print(top10, size);
	delete[] top10;
	return true;
}

void Run(HANDLE &hConsole, WalletsContainer & wallets, TransactionsContainer & transacions, OrdersContainer & orders)
{
	char errmsg[100];
	char command[50];
	bool running = true;
	do
	{
		cout << ">";
		cin >> command;

		if (strcmp(command, COMMAND_ADD_WALLET) == 0)
		{
			double fiatmoney;
			char name[256];
			cin >> fiatmoney;
			cin.ignore();
			cin.getline(name, 256, '\n');
			
			bool success = AddWallet(fiatmoney, name, wallets, errmsg);
			Transaction t;
			t.senderId = SYSTEM_WALLET;
			t.receiverId = wallets.arr[wallets.index - 1].id;
			t.fmiCoins = fiatmoney / MONEY_TO_FMICOINT_COURCE;
			t.time = time(NULL);
			success = success && AddTransaction(transacions, t);
			if (success)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_GREEN);
				cout << "Wallet Added." << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			else
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << errmsg << endl;
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
			Order o;
			o.fmiCoins = coins;
			o.walletId = id;
			o.time = time(NULL);
			bool conversion = CharArrayToType(type, o.type);
			Wallet w;
			bool found = TellWalletById(o.walletId, wallets, w, transacions, errmsg);
			if (!found)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << errmsg << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
				continue;
			}
			bool feasible = IsFeasible(o, w);
			if (!feasible)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << "Order not feasible" << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
				continue;
			}
			if (!conversion)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << "Conversion from Char Array to Type failed" << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
				continue;
			}
			bool success = AddOrder(o, orders, transacions, wallets, errmsg);
			if (success)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_GREEN);
				cout << "New order created." << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			else
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << errmsg << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			continue;
		}

		if (strcmp(command, COMMAND_WALLET_INFO) == 0)
		{
			unsigned int id;
			cin >> id;
			bool success = WalletInfo(id, wallets, transacions, errmsg);
			if (!success)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << errmsg << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			continue;
		}

		if (strcmp(command, COMMAND_ATTRACT_INVESTORS) == 0)
		{
			bool success = AttractInvestors(wallets, transacions, errmsg);
			if (!success)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_RED);
				cout << errmsg << endl;
				SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
			}
			continue;
		}

		running = strcmp(command, COMMAND_QUIT) != 0;
		// if the command is not quit and we reached this code => the command is invalid
		if (running)
		{
			SetConsoleTextAttribute(hConsole, CONSOLE_RED);
			cout << "Invalid command" << endl;
			SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
		}
	} while (running);
	bool saved = Save(wallets, transacions, orders, errmsg);
	if (!saved)
	{
		SetConsoleTextAttribute(hConsole, CONSOLE_RED);
		cout << errmsg << endl;
		SetConsoleTextAttribute(hConsole, CONSOLE_DEFAULT);
	}
}

void Load(WalletsContainer & wallets, TransactionsContainer & transactions, OrdersContainer & orders)
{
	char msg[100];
	// todo
	bool success = LoadWallets(wallets, msg);
	if (!success)
	{
		cout << msg << endl;
	}
	// for debug purposes
	cout << "Wallets: " << wallets.index << endl;
	Print(wallets);
	success = LoadTransactions(transactions, msg);
	if (!success)
	{
		cout << msg << endl;
	}
	// for debug purposes
	cout << "Transactions: " << transactions.index << endl;
	Print(transactions);
	success = LoadOrders(orders, msg);
	if (!success)
	{
		cout << msg << endl;
	}
	// for debug purposes
	cout << "Orders: " << orders.index << endl;
	Print(orders);
}

int main()
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // console's handle - for changing text color

	WalletsContainer wallets;
	TransactionsContainer transactions;
	OrdersContainer orders;
	Load(wallets, transactions, orders);

	Run(hConsole, wallets, transactions, orders);

	delete[] wallets.arr;
	delete[] transactions.arr;
	delete[] orders.arr;
	return 0;
}