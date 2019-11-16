#pragma once
#include <string>

struct QInt {
	int data[4] = { 0 };
};

void ScanQInt(QInt &x);
void PrintQInt(QInt x);
bool* DecToBin(QInt x);
QInt BinToDec(bool *bin);
std::string BinToHex(bool *bin);
std::string DecToHex(QInt x);
QInt operator+ (const QInt &a, const QInt &b);