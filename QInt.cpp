#include "QInt.h"
#include <iostream>
#include <string>
#include <cmath>
#include <memory>

#define SIZE 128

using namespace std;

void OneComplement(bool bin[], uint32_t size)
{
	uint32_t i = 0;

	for (; i < size; i++)
		bin[i] = !bin[i];
}

void TwoComplement(bool bin[], uint32_t size)
{
	uint32_t i = 0;

	OneComplement(bin, size);

	// cộng 1
	i = 0;
	bin[i] ^= 1; 
	while (bin[i] == 0 && i < size - 1)
	{
		bin[i + 1] ^= 1;
		i++;
	}
}

bool IsNumber(string s)
{
	uint32_t i = s.front() == '-' ? 1 : 0;
	while (i < s.length() && isdigit(s[i])) ++i;
	return !s.empty() && i == s.length();
}

void CleanUpString(string &str)
{
	if (!str.empty() && str.front() != '-')
		while (!str.empty() && *str.begin() == '0')
			str.erase(str.begin());
	else
		while (!str.empty() && *str.begin() + 1 == '0')
			str.erase(str.begin() + 1);
}

// for addition of same signed string only!
string SumNumberString(string s1, string s2)
{
	string value;
	int carry = 0;

	uint32_t i = 0;
	string reverse;

	while (i < s1.size() || i < s2.size() || carry != 0)
	{
		// cộng chỉ số thứ i của chuỗi s1 và s2
		if (i < s1.size() && i < s2.size())
			carry += ((s1[s1.size() - 1 - i] - '0') + (s2[s2.size() - 1 - i] - '0'));
		else if (i < s1.size() && i >= s2.size())
			carry += s1[s1.size() - 1 - i] - '0';
		else if (i >= s1.size() && i < s2.size())
			carry += s2[s2.size() - 1 - i] - '0';

		++i;

		// lưu kết quả vào 1 chuỗi, chuỗi này sẽ bị ngược
		reverse.push_back((carry % 10) + '0');
		carry /= 10;
	}

	// đảo ngược chuỗi
	for (string::reverse_iterator rit = reverse.rbegin(); rit != reverse.rend(); rit++)
		value.push_back(*rit);


	// rút gọn
	if (!value.empty())
		while (!value.empty() && *value.begin() == '0')
			value.erase(value.begin());

	return value;
}

string* CreateConversion(uint32_t size)
{
	// lưu giá trị của pow(2,i);
	string *convert = new string[size];
	convert[0] = '1';

	// convert[i] = 2 * convert[i - 1];
	for (uint32_t i = 1; i < size; i++)
		convert[i] = SumNumberString(convert[i - 1], convert[i - 1]);

	return convert;
}

// chia chuoi so cho 2
string DivideBy2(const string &s)
{
	string str;
	int temp = 0;
	uint32_t i = 0;

	if (s.front() == '-')
	{
		str.push_back('-');
		i = 1;
	}

	for (; i < s.size(); i++)
	{
		temp = temp * 10 + (s[i] - '0');// Lấy trị số thứ i của chuỗi số
		str.push_back((temp / 2) + '0');// lưu phần nguyên của temp / 2 vào chuỗi mới
		temp %= 2;
	}

	CleanUpString(str);

	return str;
}

void StringToBinary(string &s, bool bin[])
{
	uint32_t i = 0;

	while (!s.empty() && i < SIZE - 1)
	{
		// chia chuỗi số, lưu số dư vào chuỗi nhị phân
		int last = s.back() - '0';
		bin[i++] = last % 2;
		s = DivideBy2(s);
	}
}

string BinaryToString(bool bin[], string convert[])
{
	string s;

	if (bin[SIZE - 1] == 1)
	{
		s.push_back('-');
		TwoComplement(bin, SIZE);
	}

	string temp = "0";
	for (uint32_t i = 0; i < SIZE - 1; i++)
		if (bin[i])
			temp = SumNumberString(temp, convert[i]);

	s.append(temp);

	return s;
}

QInt::QInt()
{
	for (uint32_t i = 0; i < 4; i++)
		data[i] = 0;
}

QInt::QInt(string decimal)
{
	bool bin[SIZE] = { false };
	char sign = decimal.front();

	StringToBinary(decimal, bin);

	if (sign == '-')
		TwoComplement(bin, SIZE);

	*this = BinToDec(bin);
}

QInt QInt::BinToDec(bool *bin)
{
	QInt x;

	for (uint32_t i = 0; i < SIZE; i++)
		x.data[i / 32] |= (bin[i] << i);

	return x;
}

//Possible memory leak, remember to delete
bool* QInt::DecToBin() const
{
	bool* bin = new bool[SIZE];

	for (unsigned i = 0; i < SIZE; i++)
	{
		bin[i] = false;
		bin[i] = (this->data[i / 32] >> i) & 1;
	}

	return bin;
}

string QInt::BinToHex(bool *bin)
{
	string hex;

	if (bin[SIZE - 1] == 1)
	{
		hex.push_back('-');
		TwoComplement(bin,SIZE);
	}

	uint32_t i = 0;
	int carry = 0;
	string temp;
	for (; i < SIZE - 4; i+=4)
	{
		for (uint32_t j = 0; j < 4; j++)
			if (bin[i + j] == 1)
				carry += (int)(pow(2, j));

		switch (carry)
		{
		case 0: 
			temp.push_back('0');
			break; 
		case 1: 
			temp.push_back('1');
			break; 
		case 2: 
			temp.push_back('2');
			break; 
		case 3: 
			temp.push_back('3');
			break; 
		case 4: 
			temp.push_back('4');
			break;
		case 5: 
			temp.push_back('5');
			break; 
		case 6: 
			temp.push_back('6');
			break; 
		case 7: 
			temp.push_back('7');
			break; 
		case 8: 
			temp.push_back('8');
			break; 
		case 9: 
			temp.push_back('9');
			break; 
		case 10: 
			temp.push_back('A');
			break; 
		case 11: 
			temp.push_back('B');
			break; 
		case 12: 
			temp.push_back('C');
			break; 
		case 13: 
			temp.push_back('D');
			break; 
		case 14: 
			temp.push_back('E');
			break; 
		case 15: 
			temp.push_back('F');
			break; 
		default:
			break;
		}

		carry = 0;
	}

	
	for (uint32_t j = 0; j < 3; j++)
		if (bin[i + j] == 1)
			carry += (int)(pow(2, j));

	switch (carry)
	{
	case 0: 
		temp.push_back('0');
		break; 
	case 1: 
		temp.push_back('1');
		break; 
	case 2: 
		temp.push_back('2');
		break; 
	case 3: 
		temp.push_back('3');
		break; 
	case 4: 
		temp.push_back('4');
		break; 
	case 5: 
		temp.push_back('5');
		break; 
	case 6: 
		temp.push_back('6');
		break; 
	case 7: 
		temp.push_back('7');
		break; 
	}	

	for (string::reverse_iterator rit = temp.rbegin(); rit != temp.rend(); rit++)
		hex.push_back(*rit);
	CleanUpString(hex);
	return hex;
}

string QInt::DecToHex()
{
	bool *bin = this->DecToBin();
	string hex = BinToHex(bin);
	delete[]bin;
	return hex;
}

void QInt::ScanQInt()
{
	string s;
	cin >> s;
	if (IsNumber(s)) {
		bool bin[SIZE] = { false };
		uint32_t i = 0;
		char sign = s.front();

		StringToBinary(s, bin);

		// nếu chuỗi số âm thì lấy bù 2 của dãy nhị phân bin
		if (sign == '-')
			TwoComplement(bin, SIZE);

		for (i = 0; i < 4; i++)
			this->data[i] = 0;

		*this = BinToDec(bin);
	}
}

void QInt::PrintQInt()
{
	bool bin[SIZE] = { false };
	uint32_t i;
	string output;

	for (i = 0; i < SIZE; i++)
		bin[i] = this->DecToBin()[i];

	// lưu giá trị của pow(2,i);
	string *convert = CreateConversion(SIZE - 1);
	
	output = BinaryToString(bin, convert);
	delete[]convert;

	cout << output;
}

QInt operator+ (const QInt &a, const QInt &b)
{
	bool *bin1 = a.DecToBin();
	bool *bin2 = b.DecToBin();
	bool bin[SIZE];

	for (uint32_t i = 0; i < SIZE; i++)
	{
		if (bin1[i] && bin2[i])
		{
			uint32_t j = i;

			bin1[j] ^= bin2[j];

			while (bin1[j] == 0 && j < SIZE - 1)
			{
				bin1[j + 1] ^= 1;
				j++;
			}
			bin[i] = bin1[i];
			continue;
		}
		bin1[i] ^= bin2[i];
		bin[i] = bin1[i];
	}

	delete[]bin1;
	delete[]bin2;

	return QInt::BinToDec(bin);
}

QInt operator-(const QInt &a, const QInt &b)
{
	bool *bin2 = b.DecToBin();
	TwoComplement(bin2, SIZE);
	return a + QInt::BinToDec(bin2);
}

QInt operator*(const QInt &a, const QInt &b)
{
	bool* bin1 = a.DecToBin();
	bool* bin2 = b.DecToBin();
	QInt x;

	for (uint32_t i = 0; i < SIZE; i++)
	{
		if (bin2[i])
		{
			uint32_t j;
			bool bin[SIZE];
			for (j = 0; j < i; j++)
				bin[j] = 0;
			for (j = i; j < SIZE; j++)
				bin[j] = bin1[j - i];

			x = x + QInt::BinToDec(bin);
		}
	}

	return x;
}

QInt operator+=(QInt &a, const QInt &b)
{
	a = a + b;
	return a;
}

QInt operator &(const QInt& a, const QInt& b)
{
	QInt temp;
	for (int i = 0; i < 4; i++)
	{
		temp.data[i] = a.data[i] & b.data[i];
	}
	return temp;
}

QInt operator |(const QInt& a, const QInt& b)
{
	QInt temp;
	for (int i = 0; i < 4; i++)
	{
		temp.data[i] = a.data[i] | b.data[i];
	}
	return temp;
}

QInt operator ^(const QInt& a, const QInt& b)
{
	QInt temp;
	for (int i = 0; i < 4; i++)
	{
		temp.data[i] = a.data[i] ^ b.data[i];
	}
	return temp;
}

//QInt operator << (const QInt& a, const QInt& b)
//{
//	QInt temp = a;
//	QInt one;
//	bool c[3];
//	QInt i;
//	const uint64_t max = pow(2, 31);
//	string bit1 = "00000000000000000000000000000001";
//	uint32_t mask = stoul(bit1, nullptr, 2);
//	for (i; i < b; i += one)
//	{
//		temp.data[0] <<= 1;
//		check[0] = (temp.data[1] >= max) ? true : false;
//		temp.data[1] <<= 1;
//		check[1] = (temp.data[2] >= max) ? true : false;
//		temp.data[2] <<= 1;
//		check[2] = (temp.data[3] >= max) ? true : false;
//		temp.data[3] <<= 1;
//
//		if (check[0]) 
//		{
//			temp.data[0] |= mask;
//		}
//		if (check[1]) 
//		{
//			temp.data[1] |= mask;
//		}
//		if (check[2]) 
//		{
//			temp.data[2] |= mask;
//		}
//	}
//	return temp;
//}
//
//QInt operator >> (const QInt& a, const QInt& b)
//{
//	QInt temp = a;
//	QInt one;
//	bool c[3];
//	QInt i;
//	const uint64_t min = 1;
//	string bit1 = "10000000000000000000000000000000";
//	uint32_t mask = stoul(bit1, nullptr, 2);
//	for (i; i < b; i += one)
//	{
//		temp.data[0] >>= 1;
//		check[0] = (temp.data[1] >= min) ? true : false;
//		temp.data[1] >>= 1;
//		check[1] = (temp.data[2] >= min) ? true : false;
//		temp.data[2] >>= 1;
//		check[2] = (temp.data[3] >= min) ? true : false;
//		temp.data[3] >>= 1;
//
//		if (check[0])
//		{
//			temp.data[0] |= mask;
//		}
//		if (check[1])
//		{
//			temp.data[1] |= mask;
//		}
//		if (check[2])
//		{
//			temp.data[2] |= mask;
//		}
//	}
//	return temp;
//}

QInt QInt::operator~()
{
	QInt temp;
	for (int i = 0; i < 4; i++)
	{
		temp.data[i] = ~this->data[i];
	}
	return temp;
}
QInt QInt::operator=(const QInt& a)
{
	if (this != &a) {
		for (int i = 0; i < 4; i++) {
			this->data[i] = a.data[i];
		}
	}
	return *this;
}
bool QInt::operator>(const QInt& a)
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] > a.data[i];
	}
}
bool QInt::operator>=(const QInt& a)
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] >= a.data[i];
	}
}
bool QInt::operator<(const QInt& a)
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] < a.data[i];
	}
}
bool QInt::operator<=(const QInt& a)
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] <= a.data[i];
	}
}
bool QInt::operator==(const QInt& a)
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] == a.data[i];
	}
}