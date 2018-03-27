#pragma once
struct Wallet
{
	char owner[256];
	unsigned int id;
	double fiatMoney;
	double fmiCoins;
	int ordersCount;
	bool fmiCoinsCalculated = false;
	long long firstTransaction;
	long long lastTransaction;
	static const int WRITE_SIZE = sizeof(char) * 256 + sizeof(double) + sizeof(unsigned); // size of Wallet written in wallets.dat
};

struct Transaction
{
	long long time;
	unsigned int senderId;
	unsigned int receiverId;
	double fmiCoins;
	static const int WRITE_SIZE = sizeof(unsigned) * 2 + sizeof(long long) + sizeof(double);
};

enum Type
{
	SELL = 0,
	BUY = 1
};

struct Order
{
	Type type;
	unsigned walletId;
	double fmiCoins;
	bool satisfied;
	long long time;
	static const int WRITE_SIZE = sizeof(unsigned) + sizeof(Type) + sizeof(double);
};

struct WalletsContainer
{
	Wallet * arr;
	long long size;
	long long index;
};

struct OrdersContainer
{
	Order * arr;
	long long size;
	long long index;
};
