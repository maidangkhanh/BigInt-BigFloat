#include "QInt.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>


using namespace std;

void TwoComplement(bool bin[], unsigned int size)
{
	unsigned int i = 0;
	
	// lấy bù 1
	for (; i < size; i++)
		bin[i] = !bin[i]; 

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
	unsigned int i = s.front() == '-' ? 1 : 0;
	while (i < s.length() && isdigit(s[i])) ++i;
	return !s.empty() && i == s.length();
}

// chia chuoi so cho 2
string DivideBy2(const string &s)
{
	string str;
	int temp = 0;
	unsigned int i = 0;

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

	if (!str.empty() && str.front() != '-')
		while (!str.empty() && *str.begin() == '0')
			str.erase(str.begin());
	else
		while (!str.empty() && *str.begin() + 1 == '0')
			str.erase(str.begin() + 1);

	return str;
}

void ScanQInt(QInt &x)
{
	string s;
	cin >> s;
	if (IsNumber(s)) {
		bool bin[128] = { false };
		unsigned int i = 0;
		char sign = s.front();

		while (!s.empty() && i < 127)
		{
			// chia chuỗi số, lưu số dư vào chuỗi nhị phân
			int last = s.back() - '0';
			bin[i++] = last % 2;
			s = DivideBy2(s);
		}

		// nếu chuỗi số âm thì lấy bù 2 của dãy nhị phân bin
		if (sign == '-')  // BUG
			TwoComplement(bin, 128);

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

void PrintQInt(QInt x)
{
	bool bin[128] = { false };
	unsigned int i;

	for (i = 0; i < 128; i++)
		bin[i] = (x.data[i / 32] >> i) & 1;

	// lưu giá trị của pow(2,i);
	string convert[127]; 
	convert[0] = '1';

	// convert[i] = 2 * convert[i - 1];
	for (i = 1; i < 127; i++)
		convert[i] = SumNumberString(convert[i - 1], convert[i - 1]);
	
	string output;

	if (bin[127] == 1)
	{
		output.push_back('-');
		TwoComplement(bin,128);
	}

	string temp = "0";
	for (i = 0; i < 127; i++)
		if (bin[i])
			temp = SumNumberString(temp, convert[i]);

	output.append(temp);

	cout << output << endl;
}