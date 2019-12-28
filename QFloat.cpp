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
	unsigned int i = s.front() == '-' ? 1 : 0; // set index
	bool flag = false;
	while (i < s.length() && isdigit(s[i]))
	{
		if ((s[i + 1] == '.') ^ (flag)) // index at '.' skip over 
		{
			flag = true;
			i++;
		}
		i++;
	}
	return !s.empty() && i == s.length();

}

void fCleanUpString(string &str) // clear 0-bits at begining of bit-string 
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

string NaturalDecimalStringToBinaryString(string s) // chuyển phần nguyên chuỗi cơ số 10 sang cơ số 2
{
	string result = "";
	string temp = s;
	while (!temp.empty()) {
		int32_t n = temp.length();
		result += ((temp[n - 1] - '0') % 2 ? "1" : "0"); // lấy số dư khi chia chuỗi cho 2
		temp = fDivideBy2(temp); // chia chuỗi cho 2
		while (!temp.empty() && (temp[0] == '0'))
			temp.erase(temp.begin());
	}
	reverse(result.begin(), result.end()); // đảo ngược chuỗi
	return result;
}

string FractionDecimalStringToBinaryString(string s) // chuyển phần thập phân chuỗi cơ số 10 sang cơ số 2
{
	s = '0' + s;
	string res = "";
	for (int i = 0; i < FRACTION; i++)
	{
		s = fSumNumberString(s, s); // nhân 2
		res = res + s[0];			// cộng phần nguyên của tích trên
		s[0] = '0';					// đặt phân nguyên là 0
	}
	return res;
}

//possible memory leak, remember to delete
bool* StringToFloatingPointBinary(string s) //chuỗi cơ số 10 sang mảng nhị phân dấu chấm động
{
	string sign = s.front() == '-' ? "1" : "0";
	string natural = "", fraction = "";
	int point_pos = s.find('.');

	if (point_pos == -1) // nếu không tìm được dấu chấm
	{
		string temp = s;
		if (sign == "1")
			temp = s.substr(1);
		natural = temp; // phần nguyên là chuỗi
		fraction = "0"; // phần thập phân là 0
	}
	else
	{
		int start = 0; // đặt vị trí xét đầu tiên
		if (sign == "1")
			start = 1;
		natural = s.substr(start, point_pos - start); // phần nguyên từ vị trí đầu đến dấu chấm
		fraction = s.substr(point_pos + 1); // phần thập phân tính từ sau vị trí dấu chấm
	}
	if (natural == "") natural = "0";
	if (fraction == "") fraction = "0";

	natural = NaturalDecimalStringToBinaryString(natural); // chuyển cơ số 10 sang cơ số 2
	fraction = FractionDecimalStringToBinaryString(fraction);// chuyển cơ số 10 sang sơ số 2

	int shift = 0;
	int n_bit_1_pos = natural.find("1"); // tìm vị trí xuất hiện đầu tiên của 1-bit trong chuỗi nguyên
	int f_bit_1_pos = fraction.find("1");// tìm vị trí xuất hiện đầu tiên của 1-bit trong chuỗi thập phân

	if (n_bit_1_pos != -1) //nếu tìm thấy 1-bit trong chuỗi nguyên
	{
		shift = natural.length() - n_bit_1_pos - 1; // độ dời dấu chấm động
		fraction = natural.substr(n_bit_1_pos + 1) + fraction; // cập nhật phần thập phân sau khi dời dấu chấm động
	}
	else // nếu phần nguyên là 0
	{
		if (f_bit_1_pos == -1) // nếu phần thập phân là 0
		{
			bool *zero = new bool[SIZE];
			for (int i = 0; i < SIZE; i++)
				zero[i] = 0;
			return zero; // trả về 0
		}
		fraction.erase(0, f_bit_1_pos + 1); // dịch 1-bit đầu tiên sang phần nguyên
		shift = -(f_bit_1_pos + 1); // cập nhật độ dời dấu chấm động
	}

	string exponent = NaturalDecimalStringToBinaryString(to_string(shift + MAX_EXP)); // tạo chuỗi số mũ dựa trên độ dời dấu chấm động

	if (exponent.length() < EXPONENT) // có thể độ dài chuỗi ngắn hơn 1 so với độ dài qui định
		exponent = "0" + exponent;
	
	fraction.resize(FRACTION, '0'); // độ dài phần trị có thể ngắn hoặc dài hơn qui định, nếu ngắn hơn thì phần được chèn vào mang giá trị 0

	string bin = (sign + exponent + fraction); // tổng hợp chuỗi

	bool* bit = new bool[SIZE];

	for (int i = 0; i < SIZE; i++)
		bit[i] = bin[i] == '0' ? 0 : 1; // chuyển thành mảng bool
	
	return bit;
}

string BinaryStringToDecimalString(string x) //chuyển phần nguyên chuỗi cơ số 2 sang cơ số 10
{
	int len = x.length();
	string pow2 = "1"; // giá trị của 1-bit ở vị trí <i>
	string val = "0"; // kết quả của chuỗi sau khi chuyển đổi
	for (int i = len - 1; i >= 0; i--)
	{
		if (x[i] == '1') // nếu tại i la 1-bit
			val = fSumNumberString(val, pow2); // kết quả cộng thêm giá trị của 1-bit ở vị trí <i>
		pow2 = fSumNumberString(pow2, pow2); // giá trị của 1-bit ở vị trí <i>  = 2* giá trị của 1-bit ở vị trí <i-1>
	}
	return val;
}

string BinaryStringToFloatingPointDecimalString(string b) // chuyển phần thập phân chuỗi cơ số 2 sang cơ số 10
{
	int len = b.length();
	string temp = "5"; // lưu giữ giá trị thặp phân
	vector<string> arr; // mảng tổng hợp giá trị thập phân chuyển đổi được
	for (int i = 0; i < len; i++)
	{
		if (b[i] == '1') // nếu tại <i> là 1-bit
			arr.push_back(temp); // lưu giữ giá trị của 2^(-i)
		temp = fDivideBy2(temp+"0"); // 2^(-i) = 2^(-i+1) / 2
	}
	int len_max = 0; // độ dài tối đa của phần thập phân
	if (!arr.empty())
		len_max = arr[arr.size() - 1].length();
	string res = "0";
	for (unsigned int i = 0; i < arr.size(); i++)
	{
		arr[i].resize(len_max, '0');
		res = fSumNumberString(res, arr[i]); // cộng các giá trị thập phân đã lưu trữ
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
		if (bin_array[i])
			val += pow2;
		pow2 *= 2;
	}
	string a, b;
	val -= MAX_EXP; // phần lũy thừa
	if (val >= 0) // nếu lũy thừa không âm
	{
		string s = "1";
		for (int i = 0; i < val; i++)
		{
			s += bin_array[EXPONENT + 1 + i] + '0'; // xác định giá trị nhị phân của phần nguyên
		}

		a = BinaryStringToDecimalString(s); // chuyển đổi nhị phân sang thập phân
		b = "";
		for (int i = EXPONENT + 1 + val; i < SIZE; i++)
		{
			b += bin_array[i] + '0'; // xác định giá trị nhị phân của phần thập phân
		}
		b = BinaryStringToFloatingPointDecimalString(b); // chuyển đổi nhị phân sang thập phân
	}
	else
	{
		a = "0"; // phần nguyên là 0
		b = string(-val, '0'); // di chuyển dấu chấm động
		b[-val - 1] = '1'; // bit trước dấu chấm động là 1-bit
		for (int i = EXPONENT; i < SIZE; i++)
		{
			b += bin_array[i] + '0';// xác định giá trị nhị phân của phần thập phân
		}
		b = BinaryStringToFloatingPointDecimalString(b);// chuyển đổi cơ số 2 sang cơ số 10
	}
	cout << (bin_array[0] == true ? "-" : "") << a << "." << b;
	delete[] bin_array;
}

bool QFloat::isZero() // kiểm tra số 0
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

bool QFloat::isINF() // kiểm tra số vô cực
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

bool QFloat::isNaN() // kiểm tra Not a Number
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

bool QFloat::isDenormalisedNum() // kiểm tra số không chuẩn
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


bool QFloat::isSpecialNum() // kiểm tra số đặc biệt
{
	bool isSpecial = false;
	if (this->isZero())
	{
		cout << "0";
		isSpecial = true;
	}
	else if (this->isINF())
	{
		cout << "Infinity";
		isSpecial = true;
	}
	else if (this->isNaN())
	{
		cout << "NaN";
		isSpecial = true;

	}
	else if (this->isDenormalisedNum())
	{
		cout << "Dernomalised number";
		isSpecial = true;
	}
	return isSpecial;
}

void ExecuteQFloat(string in, string out)
{
	freopen(in.c_str(), "w", stdout);
	freopen(out.c_str(), "r", stdin);
	
	int base1, base2;
	while (cin >> base1)
	{
		cin >> base2;
		QFloat x;
		if (base1 == 2)
		{
			string bin;
			cin >> bin;
			bool *bit = new bool[SIZE];
			bit = StringToFloatingPointBinary(bin);
			x.BinToDec(bit);
			bool flag = x.isSpecialNum();
			if (!flag)
				x.PrintQFloat();
			delete[]bit;
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

void ExcuteQFloat()
{
	int base1, base2;
	cout << "Enter base 1: ";
	while (cin >> base1)
	{
		cout << "Enter base 2: ";
		cin >> base2;
		QFloat x;
		if (base1 == 2)
		{
			string bin;
			cout << "Enter value: ";
			cin >> bin;
			bool *bit = new bool[SIZE];
			bit = StringToFloatingPointBinary(bin);
			x.BinToDec(bit);
			bool flag = x.isSpecialNum();
			if (!flag)
				x.PrintQFloat();
			delete[]bit;
		}
		else
		{
			string dec;
			cout << "Enter value: ";
			cin >> dec;
			x.ScanQFloat(dec);
			x.PrintQFloat();
		}
		cout << endl;
	}
}