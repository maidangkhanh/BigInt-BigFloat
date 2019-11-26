#define _CRT_SECURE_NO_WARNINGS
#include "QFloat.h"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdio>

#define SIZE 128
#define EXPONENT 15
#define FRACTION 112
#define MAX_EXP 16383

using namespace std;


bool isFloat(string s)
{
	unsigned int i = s.front() == '-' ? 1 : 0;
	bool flag = false;
	while (i < s.length() && isdigit(s[i]))
	{
		if ((s[i + 1] == '.') ^ (flag))
		{
			flag = true;
			i++;
		}
		i++;
	}
	return !s.empty() && i == s.length();

}

void fCleanUpString(string &str)
{
	if (!str.empty() && str.front() != '-')
		while (!str.empty() && *str.begin() == '0')
			str.erase(str.begin());
	else
		while (!str.empty() && *str.begin() + 1 == '0')
			str.erase(str.begin() + 1);
}

string fDivideBy2(const string &s)
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

	fCleanUpString(str);

	return str;
}

string fSumNumberString(string s1, string s2)
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


	return value;
}

string NaturalDecimalStringToBinaryString(string s)
{
	string result = "";
	string temp = s;
	while (!temp.empty()) {
		int32_t n = temp.length();
		result += ((temp[n - 1] - '0') % 2 ? "1" : "0");
		temp = fDivideBy2(temp);
		while (!temp.empty() && (temp[0] == '0'))
			temp.erase(temp.begin());
	}
	reverse(result.begin(), result.end());
	return result;
}

string FractionDecimalStringToBinaryString(string s)
{
	s = '0' + s;
	string res = "";
	for (int i = 0; i < FRACTION; i++)
	{
		s = fSumNumberString(s, s);
		res = res + s[0];
		s[0] = '0';
	}
	return res;
}

//possible memory leak, remember to delete
bool* StringToFloatingPointBinary(string s)
{
	string sign = s.front() == '-' ? "1" : "0";
	string natural = "", fraction = "";
	int point_pos = s.find('.');

	if (point_pos == -1)
	{
		string temp = s;
		if (sign == "1")
			temp = s.substr(1);
		natural = temp;
		fraction = "0";
	}
	else
	{
		int start = 0;
		if (sign == "1")
			start = 1;
		natural = s.substr(start, point_pos - start);
		fraction = s.substr(point_pos + 1);
	}
	if (natural == "") natural = "0";
	if (fraction == "") fraction = "0";

	natural = NaturalDecimalStringToBinaryString(natural);
	fraction = FractionDecimalStringToBinaryString(fraction);
	int shift = 0, n_bit_1_pos = natural.find("1"), f_bit_1_pos = fraction.find("1");

	if (n_bit_1_pos != -1)
	{
		shift = natural.length() - n_bit_1_pos - 1;
		fraction = natural.substr(n_bit_1_pos + 1) + fraction;
	}
	else
	{
		if (f_bit_1_pos == -1)
		{
			bool *zero = new bool[SIZE];
			for (int i = 0; i < SIZE; i++)
				zero[i] = 0;
			return zero;
		}
		fraction.erase(0, f_bit_1_pos + 1);
		shift = -(f_bit_1_pos + 1);
	}

	string exponent = NaturalDecimalStringToBinaryString(to_string(shift + MAX_EXP));

	if (exponent.length() < EXPONENT)
		exponent = "0" + exponent;
	
	fraction.resize(FRACTION, '0');

	string bin = (sign + exponent + fraction);

	bool* bit = new bool[SIZE];

	for (int i = 0; i < SIZE; i++)
		bit[i] = bin[i] == '0' ? 0 : 1;
	
	return bit;
}

string BinaryStringToDecimalString(string x) {
	int len = x.length();
	string pow2 = "1";
	string val = "0";
	for (int i = len - 1; i >= 0; i--)
	{
		if (x[i] == '1')
			val = fSumNumberString(val, pow2);
		pow2 = fSumNumberString(pow2, pow2);
	}
	return val;
}

string BinaryStringToFloatingPointDecimalString(string b) 
{
	int len = b.length();
	string temp = "5";
	vector<string> arr;
	for (int i = 0; i < len; i++)
	{
		if (b[i] == '1')
		{
			arr.push_back(temp);
		}
		temp = fDivideBy2(temp+"0");
	}
	int len_max = 0;
	if (!arr.empty())
		len_max = arr[arr.size() - 1].length();
	string res = "0";
	for (unsigned int i = 0; i < arr.size(); i++)
	{
		arr[i].resize(len_max, '0');
		res = fSumNumberString(res, arr[i]);
	}
	return res;
}

QFloat::QFloat()
{
	for (int i = 0; i < 4; i++)
		this->data[i] = 0;
}

QFloat::QFloat(std::string s)
{
	if (isFloat(s))
	{
		bool* bin = StringToFloatingPointBinary(s);
		*this = BinToDec(bin);
		delete[]bin;
	}
}
QFloat QFloat::BinToDec(bool *bin)
{
	QFloat x;

	for (uint32_t i = 0; i < SIZE; i++)
		x.data[i / 32] |= (bin[i] << i);

	return x;
}
//Possible memory leak, remember to delete
bool* QFloat::DecToBin() const
{
	bool* bin = new bool[SIZE];

	for (unsigned i = 0; i < SIZE; i++)
	{
		bin[i] = false;
		bin[i] = (this->data[i / 32] >> i) & 1;
	}

	return bin;
}

void QFloat::ScanQFloat()
{
	string s;
	cin >> s;
	bool* bin = StringToFloatingPointBinary(s);
	*this = BinToDec(bin);
	delete[]bin;
	
}

void QFloat::ScanQFloat(string s)
{
	bool* bin = StringToFloatingPointBinary(s);
	*this = BinToDec(bin);
	delete[]bin;
}

void QFloat::PrintQFloat()
{
	bool *bin_array = this->DecToBin();
	int pow2 = 1, val = 0;
	for (int i = EXPONENT; i > 0; i--)
	{
		val += pow2 * bin_array[i];
		pow2 *= 2;
	}
	string a, b;
	val -= MAX_EXP;
	if (val >= 0)
	{
		string s = "1";
		for (int i = 0; i < val; i++)
		{
			s += bin_array[EXPONENT + 1 + i] + '0';
		}
		//cout << s << endl;

		a = BinaryStringToDecimalString(s);
		b = "";
		for (int i = EXPONENT + 1 + val; i < SIZE; i++)
		{
			b += bin_array[i] + '0';
		}
		//cout << b << endl;
		b = BinaryStringToFloatingPointDecimalString(b);
	}
	else
	{
		a = "0";
		b = string(-val, '0');
		b[-val - 1] = '1';
		for (int i = EXPONENT; i < SIZE; i++)
		{
			b += bin_array[i] + '0';
		}
		b = BinaryStringToFloatingPointDecimalString(b);
	}
	cout << (bin_array[0] == true ? "-" : "") << a << "." << b;
	delete[] bin_array;
}

bool QFloat::isZero()
{
	bool *bit = new bool[SIZE];
	bit = this->DecToBin();
	for (int i = 0; i < EXPONENT; i++)
	{
		if (bit[i] == 1)
			return false;
	}
	for (int i = EXPONENT; i < SIZE; i++)
	{
		if (bit[i] == 1)
			return false;
	}
	delete[]bit;
	return true;
}

bool QFloat::isINF()
{
	bool *bit = new bool[SIZE];
	bit = this->DecToBin();
	for (int i = 0; i < EXPONENT; i++)
	{
		if (bit[i] == 0)
			return false;
	}
	for (int i = EXPONENT; i < SIZE; i++)
	{
		if (bit[i] == 1)
			return false;
	}
	delete[]bit;
	return true;
}

bool QFloat::isNaN()
{
	bool *bit = new bool[SIZE];
	bit = this->DecToBin();
	for (int i = 0; i < EXPONENT; i++)
	{
		if (bit[i] == 0)
			return false;
	}
	for (int i = EXPONENT; i < SIZE; i++)
	{
		if (bit[i] == 1)
			return true;
	}
	delete[]bit;
	return false;
}

bool QFloat::isDenormalisedNum()
{
	bool *bit = new bool[SIZE];
	bit = this->DecToBin();
	for (int i = 0; i < EXPONENT; i++)
	{
		if (bit[i] == 0)
			return false;
	}
	for (int i = EXPONENT; i < SIZE; i++)
	{
		if (bit[i] == 1)
			return true;
	}
	delete[]bit;
	return false;
}


bool QFloat::isSpecialNum()
{
	bool isSpecial = false;
	if (this->isZero())
	{
		cout << "0";
		isSpecial = true;
	}
	else if (this->isINF())
	{
		cout << "INF";
		isSpecial = true;
	}
	else if (this->isDenormalisedNum())
	{
		cout << "Denormalised Number";
		isSpecial = true;
	}
	else if (this->isNaN())
	{
		cout << "NaN";
		isSpecial = true;

	}
	return isSpecial;
}

void ExecuteQfloat(string in, string out)
{
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	
	int base1, base2;
	while (cin >> base1)
	{
		cin >> base2;
		QFloat x;
		if (base1 == 2)
		{
			string bin;
			cin >> bin;
			bin = BinaryStringToFloatingPointDecimalString(bin);
			bool flag = x.isSpecialNum();
			if (!flag)
			{
				x.ScanQFloat(bin);
				x.PrintQFloat();
			}
		}
		else
		{
			string dec;
			cin >> dec;
			x.ScanQFloat(dec);
			x.PrintQFloat();
		}
		cout << endl;
	}
	fclose(stdin);
	fclose(stdout);
}