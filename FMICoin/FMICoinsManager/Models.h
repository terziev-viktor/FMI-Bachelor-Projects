struct Wallet
{
	char owner[256];
	unsigned int id;
	double fiatMoney;
};

struct Transaction
{
	long long time;
	unsigned int senderId;
	unsigned int receiverId;
	double fmiCoins;
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
};
