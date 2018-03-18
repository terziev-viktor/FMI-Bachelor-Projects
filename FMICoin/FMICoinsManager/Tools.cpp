#pragma once
#include "ToolsH.h"
#include <ctime>

bool CharArrayToType(const char type[], Type & out)
{
	if (strcmp(SELL_AS_STR, type) == 0)
	{
		out = Type::SELL;
		return true;
	}
	else if(strcmp(BUY_AS_STR, type) == 0)
	{
		out = Type::BUY;
		return true;
	}
	return false;
}

void TypeToCharArray(char dest[5], Type type)
{
	if (type == Type::SELL)
	{
		strcpy_s(dest, sizeof(char) * 5, "SELL");
	}
	else
	{
		strcpy_s(dest, sizeof(char) * 5, "BUY");
	}
}

void Print(const Wallet & w, bool asInvestor = false)
{
	if (asInvestor)
	{
		char first[50], last[50];
		ctime_s(first, sizeof(char) * 50, &w.firstTransaction);
		ctime_s(last, sizeof(char) * 50, &w.lastTransaction);
		cout << "ID: " << w.id << endl;
		cout << "Coins: " << w.fmiCoins << endl;
		cout << "Transactions: " << w.transactionsCount << endl;
		cout << "First Transaction on " << first;
		cout << "Last transaction on " << last;
	}
	else
	{
		cout << "ID: " << w.id << endl;
		cout << "Fiat Money: " << w.fiatMoney << endl;
		cout << "Owner: " << w.owner << endl;
	}
}

void Print(Wallet * wallets, int size)
{
	for (int i = 0; i < size; i++)
	{
		Print(wallets[i], true);
	}
}

void Print(const Transaction & w)
{
	char timeStr[50];
	ctime_s(timeStr, sizeof(char) * 50, &w.time);
	cout << w.senderId << " sends " << w.fmiCoins << " coins to " << w.receiverId << " at " << timeStr;
}

void Print(const TransactionsContainer & t)
{
	for (long long i = 0; i < t.index; i++)
	{
		Print(t.arr[i]);
	}
}

void Print(const WalletsContainer & w)
{
	for (long long i = 0; i < w.index; i++)
	{
		Print(w.arr[i]);
	}
}

void Print(const Order & o)
{
	char type[5];
	TypeToCharArray(type, o.type);
	cout << o.walletId << " wants to " << type << " " << o.fmiCoins << " coins" << endl;
}

void Print(const OrdersContainer & o)
{
	for (long long i = 0; i < o.index; i++)
	{
		Print(o.arr[i]);
	}
}

bool IsFeasible(Order & o, Wallet & w)
{
	if (o.type == Type::BUY)
	{
		int comparison = Compare(w.fiatMoney, o.fmiCoins * MONEY_TO_FMICOINT_COURCE);
		return comparison == 1 || comparison == 0;
	}
	else
	{
		int comparison = Compare(w.fmiCoins, o.fmiCoins);
		return comparison == 1 || comparison == 0;
	}
}

void CalcFmiCoins(Wallet & w, TransactionsContainer & t)
{
	if (w.fmiCoinsCalculated)
	{
		return;
	}
	w.fmiCoins = 0;
	bool first = false;
	long long last = 0;
	w.transactionsCount = 0;
	for (int i = 0; i < t.index; i++)
	{
		if (t.arr[i].receiverId == w.id)
		{
			w.transactionsCount++;
			last = t.arr[i].time;
			w.fmiCoins += t.arr[i].fmiCoins;
			if (!first)
			{
				w.firstTransaction = t.arr[i].time;
				first = true;
			}
		}
		else if (t.arr[i].senderId == w.id)
		{
			w.transactionsCount++;
			if (!first)
			{
				first = true;
				w.firstTransaction = t.arr[i].time;
			}
			last = t.arr[i].time;
			w.fmiCoins -= t.arr[i].fmiCoins;
		}
	}
	w.lastTransaction = last;
	w.fmiCoinsCalculated = true;
}

bool TellWalletById(unsigned int id, WalletsContainer& wallets, Wallet & out, TransactionsContainer & transactions, char errmsg[100])
{
	long long p = wallets.index / 2;
	long long rightIndex = wallets.index;
	long long leftIndex = 0;
	bool found = wallets.arr[p].id == id;
	while (!found && (rightIndex - leftIndex) > 1)
	{
		if (id > wallets.arr[p].id)
		{
			leftIndex = p;
			p += (rightIndex - p) / 2;
		}
		else
		{
			rightIndex = p;
			p -= (p - leftIndex) / 2;
		}
		found = wallets.arr[p].id == id;
	}
	if (found)
	{
		Cpy(out, wallets.arr[p]);
		CalcFmiCoins(wallets.arr[p], transactions);
		return true;
	}
	strcpy_s(errmsg, sizeof(char) * 100, "Wallet not found.");
	return false;
}

int Compare(double a, double b)
{
	double c = a - b;
	if (c < eps)
	{
		return 0;
	}
	return c > 0 ? 1 : -1;
}

bool LoadWallets(WalletsContainer & w, char errmsg[100])
{
	std::ifstream in;
	in.open(WALLETS, std::ios::binary | std::ios::ate);
	if (!in.good())
	{
		in.close();
		strcpy_s(errmsg, sizeof(char) * 100, "Error on opening Wallets.dat");
		return false;
	}
	// get size
	long long gPos = in.tellg();
	in.seekg(0, std::ios::beg);
	w.size = gPos / Wallet::WRITE_SIZE;
	if (w.size == 0)
	{
		w.index = 0;
		w.size = 5;
	}
	else
	{
		w.index = w.size;
		w.size *= 2; // giving some space to work with
	}
	w.arr = new Wallet[w.size];
	for (long long i = 0; i < w.index; i++)
	{
		in.read(reinterpret_cast<char*>(&w.arr[i].id), sizeof(unsigned));
		in.read(reinterpret_cast<char*>(&w.arr[i].fiatMoney), sizeof(double));
		in.read(reinterpret_cast<char*>(&w.arr[i].owner), sizeof(char) * 256);
		// default values
		w.arr[i].fmiCoins = 0;
		w.arr[i].deleted = false;
		w.arr[i].transactionsCount = 0;
	}
	in.close();
	return true;
}

bool LoadTransactions(TransactionsContainer & t, char errmsg[100])
{
	std::ifstream in;
	in.open(TRANSACTIONS, std::ios::binary | std::ios::ate);
	if (!in.good())
	{
		in.close();
		strcpy_s(errmsg, sizeof(char) * 100, "Error on opening Transactions.dat");
		return false;
	}
	// get size
	long long gPos = in.tellg();
	in.seekg(0, std::ios::beg);
	t.size  = gPos / Transaction::WRITE_SIZE;
	if (t.size == 0)
	{
		t.size = 5;
		t.index = 0;
	}
	else
	{
		t.index = t.size;
		t.size *= 2; // we are giving some space to work
	}

	t.arr = new Transaction[t.size];
	for (long long i = 0; i < t.index; i++)
	{
		in.read(reinterpret_cast<char*>(&t.arr[i].senderId), sizeof(unsigned));
		in.read(reinterpret_cast<char*>(&t.arr[i].receiverId), sizeof(unsigned));
		in.read(reinterpret_cast<char*>(&t.arr[i].fmiCoins), sizeof(double));
		in.read(reinterpret_cast<char*>(&t.arr[i].time), sizeof(long long));
	}
	in.close();
	return true;
}

bool LoadOrders(OrdersContainer& o, char msg[100])
{
	std::ifstream in;
	in.open(ORDERS, std::ios::binary | std::ios::ate);
	if (!in.good())
	{
		strcpy_s(msg, sizeof(char) * 100, "Could not open orders.dat");
		return false;
	}
	long long gPos = in.tellg();
	o.size = gPos / Order::WRITE_SIZE;
	in.seekg(0, std::ios::beg);
	if (o.size == 0)
	{
		o.size = 5;
		o.index = 0;
	}
	else
	{
		o.index = o.size;
		o.size *= 2;
	}
	o.arr = new Order[o.size];
	for (long long i = 0; i < o.index; i++)
	{
		in.read(reinterpret_cast<char*>(&o.arr[i].walletId), sizeof(unsigned));
		in.read(reinterpret_cast<char*>(&o.arr[i].type), sizeof(Type));
		in.read(reinterpret_cast<char*>(&o.arr[i].fmiCoins), sizeof(double));
		o.arr[i].satisfied = false;
	}
	in.close();
	return true;
}

void Cpy(Wallet & dest, Wallet & source)
{
	strcpy_s(dest.owner, sizeof(char) * 256, source.owner);
	dest.id = source.id;
	dest.fiatMoney = source.id;
	dest.fmiCoins = source.fmiCoins;
	dest.transactionsCount = source.transactionsCount;
	dest.deleted = source.deleted;
	dest.firstTransaction = source.firstTransaction;
	dest.lastTransaction = source.lastTransaction;
}

void Cpy(Transaction & dest, Transaction & source)
{
	dest.time = source.time;
	dest.senderId = source.senderId;
	dest.receiverId = source.receiverId;
	dest.fmiCoins = source.fmiCoins;
}

void Cpy(Order & dest, Order & source)
{
	dest.type = source.type;
	dest.walletId = source.walletId;
	dest.fmiCoins = source.fmiCoins;
	dest.satisfied = source.satisfied;
}

void ExpandArr(WalletsContainer & w)
{
	long long new_size = w.size * 2;
	if (new_size == 0)
	{
		new_size = 5;
	}
	Wallet * new_arr = new Wallet[new_size];
	for (long long i = 0; i < w.index; i++)
	{
		Cpy(new_arr[i], w.arr[i]);
	}
	delete[] w.arr;
	w.arr = new_arr;
	w.size = new_size;
}

void ExpandArr(TransactionsContainer & t)
{
	long long new_size = t.size * 2;
	if (new_size == 0)
	{
		new_size = 5;
	}
	Transaction * new_arr = new Transaction[new_size];
	for (long long i = 0; i < t.index; i++)
	{
		Cpy(new_arr[i], t.arr[i]);
	}
	delete[] t.arr;
	t.arr = new_arr;
	t.size = new_size;
}

void ExpandArr(OrdersContainer & o)
{
	long long new_size = o.size * 2;
	if (new_size == 0)
	{
		new_size = 5;
	}
	Order * new_arr = new Order[new_size];
	for (long long i = 0; i < o.index; i++)
	{
		Cpy(new_arr[i], o.arr[i]);
	}
	delete[] o.arr;
	o.arr = new_arr;
	o.size = new_size;
}

bool SaveWallets(WalletsContainer & wallets, char errmsg[100])
{
	std::ofstream out;
	out.open(WALLETS, std::ios::binary);
	if (!out.good())
	{
		strcpy_s(errmsg, sizeof(char) * 100, "Could not save wallets");
		return false;
	}
	for (long long i = 0; i < wallets.index; i++)
	{
		out.write(reinterpret_cast<char*>(&wallets.arr[i].id), sizeof(unsigned));
		out.write(reinterpret_cast<char*>(&wallets.arr[i].fiatMoney), sizeof(double));
		out.write(reinterpret_cast<char*>(&wallets.arr[i].owner), sizeof(char) * 256);
	}
	out.close();
	return true;
}

bool SaveTransactions(TransactionsContainer & t, char errmsg[100])
{
	std::ofstream out;
	out.open(TRANSACTIONS, std::ios::binary);
	if (!out.good())
	{
		strcpy_s(errmsg, sizeof(char) * 100, "Could not save transactions");
		return false;
	}
	for (long long i = 0; i < t.index; i++)
	{
		out.write(reinterpret_cast<char*>(&t.arr[i].senderId), sizeof(unsigned));
		out.write(reinterpret_cast<char*>(&t.arr[i].receiverId), sizeof(unsigned));
		out.write(reinterpret_cast<char*>(&t.arr[i].fmiCoins), sizeof(double));
		out.write(reinterpret_cast<char*>(&t.arr[i].time), sizeof(long long));
	}
	out.close();
	return true;
}

bool SaveOrders(OrdersContainer & o, char errmsg[])
{
	std::ofstream out;
	out.open(ORDERS, std::ios::binary);
	if (!out.good())
	{
		out.close();
		strcpy_s(errmsg, sizeof(char) * 100, "Cound not open orders.dat");
		return false;
	}
	for (long long i = 0; i < o.index; i++)
	{
		if (o.arr[i].satisfied) // satisfied orders are deleted
		{
			continue;
		}
		out.write(reinterpret_cast<char*>(&o.arr[i].walletId), sizeof(unsigned));
		out.write(reinterpret_cast<char*>(&o.arr[i].type), sizeof(Type));
		out.write(reinterpret_cast<char*>(&o.arr[i].fmiCoins), sizeof(double));
	}
	out.close();
	return true;
}

bool Save(WalletsContainer & wallets, TransactionsContainer & t, OrdersContainer & o, char errmsg[100])
{
	bool success = SaveWallets(wallets, errmsg);
	success = success && SaveTransactions(t, errmsg);
	success = success && SaveOrders(o, errmsg);
	return success;
}
