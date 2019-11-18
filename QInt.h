#pragma once
#include <string>

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
	QInt operator~();
	QInt operator=(const QInt& a);
	bool operator>(const QInt& a);
	bool operator>=(const QInt& a);
	bool operator<(const QInt& a);
	bool operator<=(const QInt& a);
	bool operator==(const QInt& a);
private:
	uint32_t data[4];
};


QInt operator+(const QInt &a, const QInt &b);
QInt operator-(const QInt &a, const QInt &b);
QInt operator*(const QInt &a, const QInt &b);