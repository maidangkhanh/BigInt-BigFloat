#include "QInt.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>


using namespace std;

void TwoComplement(bool bin[])
{
	unsigned int i = 0;
	for (; i < 128; i++)
		bin[i] = !bin[i];

	bin[0] ^= 1;
	i = 0;
	while (bin[i] == 0 && i < 128)
	{
		bin[i + i] ^= 1;
		i++;
	}
}

bool IsNumber(string s)
{
	unsigned int i = s.front() == '-' ? 1 : 0;
	while (i < s.length() && isdigit(s[i])) ++i;
	return !s.empty() && i == s.length();
}

string DivideBy2(const string &s)
{
	string str;
	int temp = 0;
	for (unsigned int i = 0; i < s.size(); i++)
	{
		temp = temp * 10 + (s[i] - '0');
		str.push_back((temp / 2) + '0');
		temp %= 2;
	}

	if (!str.empty())
		while (!str.empty() && *str.begin() == '0')
			str.erase(str.begin());

	return str;
}

void ScanQInt(QInt &x)
{
	string s;
	cin >> s;
	if (IsNumber(s)) {
		bool bin[128] = { false };
		unsigned int i = 0;

		while (!s.empty() && i < 127)
		{
			int last = s.back() - '0';
			bin[i++] = last % 2;
			s = DivideBy2(s);
		}

		for (i = 0; i < 4; i++)
			x.data[i] = 0;

		for (i = 0; i < 128; i++)			
			x.data[i / 32] |= (bin[i] << i);
	}
}

// for addition of same signed string only!
string SumNumberString(string s1, string s2) 
{
	string value;
	int carry = 0;

	unsigned int i = 0;
	string reverse;

		while (i < s1.size() || i < s2.size() || carry != 0)
		{
			if (i < s1.size() && i < s2.size())
				carry += ((s1[s1.size() - 1 - i] - '0') + (s2[s2.size() - 1 - i] - '0'));
			else if (i < s1.size() && i >= s2.size())
				carry += s1[s1.size() - 1 - i] - '0';
			else if (i >= s1.size() && i < s2.size())
				carry += s2[s2.size() - 1 - i] - '0';

			++i;
			reverse.push_back((carry % 10) + '0');
			carry /= 10;
		}	

	for (string::reverse_iterator rit = reverse.rbegin(); rit != reverse.rend(); rit++)
		value.push_back(*rit);
	
	if (!value.empty())
		while (!value.empty() && *value.begin() == '0')
			value.erase(value.begin());

	return value;
}

void PrintInt(QInt x)
{
	bool bin[128] = { false };
	unsigned int i;
	bool sign = 0;

	for (i = 0; i < 128; i++)
		bin[i] = (x.data[i / 32] >> i) & 1;

	string convert[128];
	convert[0] = '1';

	for (i = 1; i < 128; i++)
		convert[i] = SumNumberString(convert[i - 1], convert[i - 1]);
	
	string output = "0";
	for (i = 0; i < 128; i++)
		if (bin[i])
			output = SumNumberString(output, convert[i]);

	cout << output << endl;
}