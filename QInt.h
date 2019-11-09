#pragma once
#include <string>

using namespace std;

struct QInt {
	int data[4] = { 0 };
};

void ScanQInt(QInt &x);
void PrintInt(QInt x);
