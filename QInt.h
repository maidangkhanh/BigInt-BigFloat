#pragma once
#include <string>

using namespace std;

struct QInt {
	int data[4] = { 0 };
};
void TwoComplement(bool bin[],unsigned int size);
void ScanQInt(QInt &x);
void PrintQInt(QInt x);
void test(QInt x);