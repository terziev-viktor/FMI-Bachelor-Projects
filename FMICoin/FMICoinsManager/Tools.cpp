#pragma once
#include "ToolsH.h"
#include <ctime>
#include <string>

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
		cout << "Orders: " << w.ordersCount << endl;
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

void CountOrdersOfWallet(Wallet & w, OrdersContainer & o)
{
	w.ordersCount = 0;
	for (int i = 0; i < o.index; i++)
	{
		if (o.arr[i].walletId = w.id)
		{
			w.ordersCount++;
		}
	}
}

bool CalcFmiCoins(Wallet & w)
{
	if (w.fmiCoinsCalculated)
	{
		return true;
	}
	std::ifstream in;
	in.open(TRANSACTIONS, std::ios::binary);
	if (!in)
	{
		in.close();
		std::ofstream a(TRANSACTIONS);
		a.close();
		in.open(TRANSACTIONS, std::ios::binary);
	}
	Transaction t;
	w.fmiCoins = 0;
	bool firstT = false;
	long long lastT;
	while (!in.eof())
	{
		in.read(reinterpret_cast<char*>(&t.senderId), sizeof(unsigned));
		in.read(reinterpret_cast<char*>(&t.receiverId), sizeof(unsigned));
		in.read(reinterpret_cast<char*>(&t.fmiCoins), sizeof(double));
		in.read(reinterpret_cast<char*>(&t.time), sizeof(long long));
		if (!in.eof())
		{
			bool isSender = w.id == t.senderId;
			bool isReciever = w.id == t.receiverId;
			if (isSender)
			{
				w.fmiCoins -= t.fmiCoins;
				if (!firstT)
				{
					w.firstTransaction = t.time;
					firstT = true;
				}
				lastT = t.time;
			}
			else if (isReciever)
			{
				w.fmiCoins += t.fmiCoins;
				if (!firstT)
				{
					w.firstTransaction = t.time;
					firstT = true;
				}
				lastT = t.time;
			}
		}
	}
	in.close();
	w.lastTransaction = lastT;
	w.fmiCoinsCalculated = true;
	return true;
}

long long search(unsigned id, WalletsContainer & wallets, long long leftIndex, long long rightIndex)
{
	if (rightIndex >= leftIndex)
	{
		long long middle = leftIndex + (rightIndex - leftIndex) / 2;

		if (wallets.arr[middle].id == id)
		{
			return middle;
		}

		if (wallets.arr[middle].id > id)
		{
			return search(id, wallets, leftIndex, middle - 1);
		}
		
		return search(id, wallets, middle + 1, rightIndex);
	}
	return -1;
}

bool TellWalletById(unsigned int id, WalletsContainer& wallets, long long & out, char errmsg[100])
{
	long long index = search(id, wallets, 0, wallets.index);
	bool found = index != -1;
	if (found)
	{
		CalcFmiCoins(wallets.arr[index]);
		out = index;
		return true;
	}
	strcpy_s(errmsg, sizeof(char) * 100, "Wallet not found.");
	return false;
}

bool TellWalletById(unsigned int id, WalletsContainer & wallets, Wallet & out, char errmsg[100])
{
	long long index = search(id, wallets, 0, wallets.index);
	bool found = index != -1;
	if (found)
	{
		CalcFmiCoins(wallets.arr[index]);
		Cpy(out, wallets.arr[index]);
		return true;
	}
	strcpy_s(errmsg, sizeof(char) * 100, "Wallet not found.");
	return false;
}

int Compare(double a, double b)
{
	double c = a - b;
	double cMod = c;
	if (cMod < 0)
	{
		cMod *= -1;
	}
	if (cMod < eps)
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
		std::ofstream a(WALLETS);
		a.close();
		in.open(WALLETS, std::ios::binary | std::ios::ate);
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
		w.arr[i].ordersCount = 0;
	}
	in.close();
	return true;
}

bool LoadOrders(OrdersContainer& o, char msg[100])
{
	std::ifstream in;
	in.open(ORDERS, std::ios::binary | std::ios::ate);
	if (!in)
	{
		in.close();
		std::ofstream a(ORDERS);
		a.close();
		in.open(ORDERS, std::ios::binary | std::ios::ate);
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
	dest.fmiCoinsCalculated = source.fmiCoinsCalculated;
	dest.fiatMoney = source.fiatMoney;
	dest.fmiCoins = source.fmiCoins;
	dest.ordersCount = source.ordersCount;
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

char * GetTransactionsTextFileName(Order & o)
{
	char fileName[50], timeStr[26];
	_itoa_s(o.walletId, fileName, 10);
	_itoa_s(o.time, timeStr, 10);
	strcat_s(fileName, timeStr);
	strcat_s(fileName, ".txt\0");
	return fileName;
}

bool WriteTransaction(WalletsContainer & wallets, Transaction & t, Order & o, char errmsg[100])
{
	Wallet sender;
	Wallet reciever;
	if (!TellWalletById(t.senderId, wallets, sender, errmsg))
	{
		return false;
	}
	if (!TellWalletById(t.receiverId, wallets, reciever, errmsg))
	{
		return false;
	}
	
	char * fileName = GetTransactionsTextFileName(o);

	std::ofstream out(fileName, std::ios::app | std::ios::out);
	if (!out)
	{
		strcpy_s(errmsg, sizeof(char) * 100, "Could not open transaction text file.");
		// return false;
	}
	out << sender.owner << " " << reciever.owner << " " << t.fmiCoins << endl;
	out.close();
	return true;
}

bool WriteTransactionMeta(int transactionsCount, double fmiCoins, Order & o, char errmsg[100])
{
	char * fileName = GetTransactionsTextFileName(o);
	std::ofstream out(fileName, std::ios::app | std::ios::out);

	if (!out)
	{
		//out.close();
		strcpy_s(errmsg, sizeof(char) * 100, "Could not open transaction meta text file.");
		// return false;
	}
	out << transactionsCount << " " << fmiCoins * MONEY_TO_FMICOINT_COURCE << endl;
	out.close();
	return true;
}

bool UpdateWallet(WalletsContainer & wallets, Transaction & t, char errmsg[100])
{
	long long wIndex;
	if (!TellWalletById(t.receiverId, wallets, wIndex, errmsg))
	{
		return false;
	}
	double tMoney = t.fmiCoins * MONEY_TO_FMICOINT_COURCE;
	wallets.arr[wIndex].fiatMoney += tMoney;
	if (!TellWalletById(t.senderId, wallets, wIndex, errmsg))
	{
		return false;
	}
	wallets.arr[wIndex].fiatMoney -= tMoney;
	wallets.arr[wIndex].fmiCoinsCalculated = false;
	return true;
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
		out.write(reinterpret_cast<char*>(&o.arr[i].walletId), sizeof(unsigned));
		out.write(reinterpret_cast<char*>(&o.arr[i].type), sizeof(Type));
		out.write(reinterpret_cast<char*>(&o.arr[i].fmiCoins), sizeof(double));
	}
	out.close();
	return true;
}

bool Save(WalletsContainer & wallets, OrdersContainer & o, char errmsg[100])
{
	bool success = SaveWallets(wallets, errmsg);
	success = success && SaveOrders(o, errmsg);
	return success;
}
