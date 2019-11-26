#pragma once
#include <string>
#include <vector>
using namespace std;
class QInt {

public:
	QInt();
	QInt(std::string decimal);
	void ScanQInt();
	void PrintQInt();
	bool* DecToBin() const;
	static QInt BinToDec(bool *bin);
	static std::string BinToHex(bool *bin);
	std::string DecToHex();
	friend QInt operator&(const QInt& a, const QInt& b);
	friend QInt operator|(const QInt& a, const QInt& b);
	friend QInt operator^(const QInt& a, const QInt& b);
	friend QInt operator<<(const QInt& a, const QInt& b);
	friend QInt operator>>(const QInt& a, const QInt& b);
	QInt ArithmeticShiftLeft(const QInt& a, const QInt& b);
	QInt ArithmeticShiftRight(const QInt& a, const QInt& b);
	QInt operator~();
	QInt operator=(const QInt& a);
	bool operator>(const QInt& a);
	bool operator>=(const QInt& a);
	bool operator<(const QInt& a);
	bool operator<=(const QInt& a);
	bool operator==(const QInt& a);
	QInt operatorrol(uint32_t bit);
	QInt operatorror(uint32_t bit);
	unsigned int getModeFromString(string a);
private:
	uint32_t data[4];
	unsigned int mode;
};


QInt operator+(const QInt &a, const QInt &b);
QInt operator-(const QInt &a, const QInt &b);
QInt operator*(const QInt &a, const QInt &b);
QInt operator/(const QInt &a, const QInt &b);

vector<string> ReadFromFile(string filename);
