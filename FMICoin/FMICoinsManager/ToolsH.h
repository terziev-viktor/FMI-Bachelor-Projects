#pragma once
#include "Models.h"
#include "IdGenerator.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
const double eps = 0.0000000001;
const char SELL_AS_STR[] = "SELL";
const char BUY_AS_STR[] = "BUY";

// out is set to Type::SELL if type == 'SELL' and to Type::BUY if type == 'BUY'
bool CharArrayToType(const char type[], Type & out);

// return 1 if a > b; return -1 if b > a and return 0 if a == b;
int Compare(double a, double b);

void Print(Wallet * wallets, int size);

void Print(const Order & o);

void Print(const OrdersContainer & o);

void Print(const Wallet & w, bool);

void Print(const Transaction & w);

void Print(const TransactionsContainer & t);

void Print(const WalletsContainer & w);

// tells if given order is feasible by the wallet provided
bool IsFeasible(Order & o, Wallet & w);

// sets out as a copy of the wallet with the given id
bool TellWalletById(unsigned int id, WalletsContainer& wallets, Wallet & out, long long & out_p, TransactionsContainer & transactions, char errmsg[100]);

bool TellWalletById(unsigned int id, WalletsContainer& wallets, Wallet & out, TransactionsContainer & transactions, char errmsg[100]);

bool LoadWallets(WalletsContainer&, char msg[100]);

bool LoadTransactions(TransactionsContainer&, char msg[100]);

bool LoadOrders(OrdersContainer&, char msg[100]);

// Calculated fmi coins of given wallet by iterating through every transaction that affects it
void CalcFmiCoins(Wallet & w, TransactionsContainer & t);

void Cpy(Wallet & dest, Wallet & source);

void Cpy(Transaction & dest, Transaction & source);

void Cpy(Order & dest, Order & source);

void ExpandArr(WalletsContainer & w);

void ExpandArr(TransactionsContainer & t);

void ExpandArr(OrdersContainer & o);

// Writes txt file with info of the transaction
bool WriteTransaction(TransactionsContainer & transactions, WalletsContainer & wallets, Transaction & t, Order & o, char errmsg[100]);

// Writes metadata for order in the end of the file for that order
bool WriteTransactionMeta(int transactionsCount, double fmiCoins, Order & o, char errmsg[100]);

// Updates fiatMoney based on transaction
bool UpdateWallet(WalletsContainer & wallets, TransactionsContainer & transactions, Transaction & t, char errmsg[100]);

bool SaveWallets(WalletsContainer & wallets, char errmsg[100]);

// Saves all data to .dat files
bool Save(WalletsContainer & wallets, TransactionsContainer & t, OrdersContainer & o ,char errmsg[100]);