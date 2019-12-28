#define _CRT_SECURE_NO_WARNINGS
#include "QInt.h"
#include <iostream>
#include <string>
#include <cmath>
#include <memory>
#include <fstream>
#include <sstream>
#define SIZE 128

using namespace std;

void OneComplement(bool bin[], uint32_t size) // bù 1
{
	uint32_t i = 0;

	for (; i < size; i++)
		bin[i] = !bin[i]; // not từng bit
}

void TwoComplement(bool bin[], uint32_t size) // bù 2
{
	uint32_t i = 0;

	OneComplement(bin, size); // lấy bù 1

	// cộng 1
	i = 0;
	bin[i] ^= 1; 
	while (bin[i] == 0 && i < size - 1)
	{
		bin[i + 1] ^= 1;
		i++;
	}
}

bool* SumBinary(bool* a, bool *b) // Cộng 2 chuỗi nhị phân
{
	for (uint32_t i = 0; i < SIZE; i++)
	{
		if (a[i] && b[i]) // Nếu cả 2 a[i] và b[i] đều là 1
		{
			uint32_t j = i;

			a[j] ^= b[j]; // a[i] + b[i]

			while (a[j] == 0 && j < SIZE - 1) // Nhớ 1
			{
				a[j + 1] ^= 1;
				j++;
			}
			continue;
		}
		a[i] ^= b[i];
	}
	return a;
}

bool IsNumber(string s) // kiểm tra chuỗi có phải chỉ chứa kí tự số nguyên
{
	uint32_t i = s.front() == '-' ? 1 : 0;
	while (i < s.length() && isdigit(s[i])) ++i;
	return !s.empty() && i == s.length();
}

void CleanUpString(string &str) // rút gọn chuỗi nhị phân
{
	if (!str.empty() && str.front() != '-') // Nếu không âm
		while (!str.empty() && *str.begin() == '0')  // Nếu bit đầu tiên của chuỗi là 0 thì xóa bit đó
			str.erase(str.begin());
	else
		while (!str.empty() && *str.begin() + 1 == '0') // Nếu âm và bit thứ 2 của chuỗi là 0 thì xóa bit đó
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

string BinaryToString(bool bin[], string convert[]) // mảng bool nhị phân chuyển thành chuỗi thập phân
{
	string s;

	if (bin[SIZE - 1] == 1)  // Nếu bit cao nhất của mảng nhị phân là 1 thì chuỗi số âm
	{
		s.push_back('-');
		TwoComplement(bin, SIZE);
	}

	string temp = "0";
	for (uint32_t i = 0; i < SIZE - 1; i++)
		if (bin[i])
			temp = SumNumberString(temp, convert[i]); // convert tại i lưu giá

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

QInt QInt::BinToDec(bool *bin) // lưu mảng nhị phân vào Qint
{
	QInt x;

	for (uint32_t i = 0; i < SIZE; i++)
		x.data[i / 32] |= (bin[i] << i);

	return x;
}

//Possible memory leak, remember to delete
bool* QInt::DecToBin() const // lấy mảng nhị phân của QInt
{
	bool* bin = new bool[SIZE];

	for (unsigned i = 0; i < SIZE; i++)
	{
		bin[i] = false;
		bin[i] = (this->data[i / 32] >> i) & 1;
	}

	return bin;
}

string QInt::BinToHex(bool *bin) // chuyển đổi từ mảng nhị phân sang thập lục phân
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
	for (; i < SIZE - 4; i+=4) // Xét từng nhóm 4 bit
	{
		for (uint32_t j = 0; j < 4; j++)
			if (bin[i + j] == 1)
				carry += (int)(pow(2, j)); // Lấy giá trị thập phân của nhóm 4 bit

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

	
	for (uint32_t j = 0; j < 3; j++) // Do có 128 bit và 1 bit dùng để biểu diễn dấu cho nên cuối cung dư lại 3 bit
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

	//Do duyệt từ bit thấp đến cao nên kết quả sẽ bị ngược -> nghịch đảo kết quả
	for (string::reverse_iterator rit = temp.rbegin(); rit != temp.rend(); rit++)
		hex.push_back(*rit);
	CleanUpString(hex);
	return hex;
}

string QInt::DecToHex() // Thập phân sang thập lục phân
{
	bool *bin = this->DecToBin(); // chuyển sang nhị phân
	string hex = BinToHex(bin); // nhị phân sang thập lục phân
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

void QInt::ScanQInt(string s)
{
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
	bool *bin1 = a.DecToBin(); // lấy chuỗi nhị phân của a và b
	bool *bin2 = b.DecToBin();

	QInt x = QInt::BinToDec(SumBinary(bin1, bin2)); // cộng 2 chuối nhị phan đó và lưu giá trị vào kết quả

	delete[]bin1;
	delete[]bin2;

	return x;
}

QInt operator-(const QInt &a, const QInt &b)
{
	bool *bin2 = b.DecToBin();
	TwoComplement(bin2, SIZE); // lấy bù 2 của số trừ
	return a + QInt::BinToDec(bin2); // lấy số bị trừ cộng cho âm số trừ
}

QInt operator*(const QInt &a, const QInt &b)
{
	bool* bin1 = a.DecToBin();
	bool* bin2 = b.DecToBin();
	QInt x;

	for (uint32_t i = 0; i < SIZE; i++)
	{
		if (bin2[i]) // nếu b[i] == 1
		{
			uint32_t j;
			bool bin[SIZE];
			for (j = 0; j < i; j++) // các bit trước i được set là 0
				bin[j] = 0;
			for (j = i; j < SIZE; j++)
				bin[j] = bin1[j - i]; // các bit từ i được set theo a[j-i] (bắt đầu từ bit nhỏ nhất tới khi bị tràn)

			x = x + QInt::BinToDec(bin); // cộng các dãy bit này lại với nhau
		}
	}

	return x;
}

QInt operator/(const QInt &a, const QInt &b)
{
	bool* dividend = a.DecToBin();
	bool* divisor = b.DecToBin();
	bool complement[SIZE] = { false };
	bool sign = dividend[SIZE - 1] ^ divisor[SIZE - 1] ? true : false;
	
	if (dividend[SIZE - 1] == true)
		TwoComplement(dividend, SIZE); // lấy trị tuyệt đối

	if (divisor[SIZE - 1] == true)
		TwoComplement(divisor, SIZE); // lấy trị tuyệt đối

	int count = 0;
	int i;
	for (i = SIZE - 2; i >= 0; i--) //xác định bit 1 cao nhất
		if (dividend[i])
		{
			count = i;
			break;
		}

	int n = count;

	while (count >= 0)
	{
		bool head = dividend[n];
		for (i = n; i >= 1; i--)
		{
			dividend[i] = dividend[i - 1];
			complement[i] = complement[i - 1];
		}
		dividend[0] = 0;
		complement[0] = head; //shift left số bị chia và phần bù

		TwoComplement(divisor, SIZE);
		SumBinary(complement, divisor); // lấy phần bù trừ số chia
		TwoComplement(divisor, SIZE);

		if (complement[SIZE - 1] == true) //nếu phần bù âm
			SumBinary(complement, divisor);// lấy phần bù cộng số chia
		else
			dividend[0] = 1; // nếu phần bù dương ghi 1 vào kết quả
			
		count--;
	}

	if (sign)
		TwoComplement(dividend, SIZE);

	QInt x = QInt::BinToDec(dividend);

	delete[]dividend;
	delete[]divisor;

	return x;
}

QInt operator +=(QInt &a, const QInt &b)
{
	a = a + b;
	return a;
}

QInt operator &(const QInt& a, const QInt& b)
{
	QInt temp;
	for (int i = 0; i < 4; i++)
	{
		temp.data[i] = a.data[i] & b.data[i]; // & each 32 bit
	}
	return temp;
}

QInt operator |(const QInt& a, const QInt& b)
{
	QInt temp;
	for (int i = 0; i < 4; i++)
	{
		temp.data[i] = a.data[i] | b.data[i];// or each 32 bit
	}
	return temp;
}

QInt operator ^(const QInt& a, const QInt& b)
{
	QInt temp;
	for (int i = 0; i < 4; i++)
	{
		temp.data[i] = a.data[i] ^ b.data[i]; //xor each 32 bit
	}
	return temp;
}

QInt operator << (const QInt& a, const QInt& b)
{
	QInt temp = a;
	QInt one("1");
	bool check[3];
	QInt i("0");
	const uint64_t max = (uint64_t)pow(2, 31);
	string bit1 = "00000000000000000000000000000001";
	uint32_t mask = stoul(bit1, nullptr, 2);
	for (i; i < b; i += one)
	{
		temp.data[0] <<= 1;// shift first 32 bit
		check[0] = (temp.data[1] >= max) ? true : false; // check bit 33rd is 1?
		temp.data[1] <<= 1;
		check[1] = (temp.data[2] >= max) ? true : false;// check bit 65th is 1?
		temp.data[2] <<= 1;
		check[2] = (temp.data[3] >= max) ? true : false;// check bit 97th is 1?
		temp.data[3] <<= 1;

		if (check[0]) 
		{
			temp.data[0] |= mask;//bit 32nd = 1 if bit 33rd = 1
		}
		if (check[1]) 
		{
			temp.data[1] |= mask;//bit 64th = 1 if bit 65th = 1
		}
		if (check[2]) 
		{
			temp.data[2] |= mask;//bit 96th = 1 if bit 95th = 1
		}
	}
	return temp;
}

QInt operator >> (const QInt& a, const QInt& b)
{
	QInt temp = a;
	QInt one("1");
	bool check[3];
	QInt i("0");
	const uint64_t min = 1;
	string bit1 = "10000000000000000000000000000000";
	uint32_t mask = stoul(bit1, nullptr, 2);
	for (i; i < b; i += one)
	{
		temp.data[0] >>= 1;
		check[0] = (temp.data[1] >= min) ? true : false;
		temp.data[1] >>= 1;
		check[1] = (temp.data[2] >= min) ? true : false;
		temp.data[2] >>= 1;
		check[2] = (temp.data[3] >= min) ? true : false;
		temp.data[3] >>= 1;

		if (check[0])
		{
			temp.data[0] |= mask;
		}
		if (check[1])
		{
			temp.data[1] |= mask;
		}
		if (check[2])
		{
			temp.data[2] |= mask;
		}
	}
	return temp;
}

QInt QInt::ArithmeticShiftLeft(const QInt& a, const QInt& b)
{
	QInt temp = a;
	uint32_t max = (uint32_t)pow(2, 31);
	temp = temp << b;
	bool check = (a.data[0] >= max) ? true : false; // check if first bit is 1 (negative number)
	if (check) 
	{
		temp.data[0] |= max;//if first bit is 1 or with max mask
	}
	else 
	{
		max -= 1;//if first bit is 0 (positive)
		temp.data[0] &= max;// and logic with and mask 0111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 
	}
	return temp;
}

//same as arithmetic shift left
QInt QInt::ArithmeticShiftRight(const QInt& a, const QInt& b)
{
	QInt temp = a;
	uint32_t max = (uint32_t)pow(2, 31);
	temp = temp >> b;
	bool check = (a.data[0] >= max) ? true : false;
	if (check)
	{
		temp.data[0] |= max;
	}
	else
	{
		max -= 1;
		temp.data[0] &= max;
	}
	return temp;
}

QInt QInt::operator~()
{
	QInt temp;
	for (int i = 0; i < 4; i++)
	{
		temp.data[i] = ~this->data[i]; // not logic each 32 bits
	}
	return temp;
}

QInt QInt::operator=(const QInt& a)
{
	if (this != &a) {
		for (int i = 0; i < 4; i++) {
			this->data[i] = a.data[i]; //assign each 32 bits
		}
	}
	return *this;
}
bool QInt::operator>(const QInt& a) 
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] > a.data[i]; //compare each 32 bits
	}
}
bool QInt::operator>=(const QInt& a)
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] >= a.data[i]; //compare each 32 bits
	}
}
bool QInt::operator<(const QInt& a)
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] < a.data[i]; //compare each 32 bits
	}
}
bool QInt::operator<=(const QInt& a)
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] <= a.data[i]; //compare each 32 bits
	}
}
bool QInt::operator==(const QInt& a)
{
	for (int i = 0; i < 4; i++) {
		return this->data[i] == a.data[i]; //compare each 32 bits
	}
}

QInt QInt::operatorrol(uint32_t bit) 
{
	bool *bin1 = new bool[bit];
	bool *bin = this->DecToBin(); //convert into binary
	for (uint32_t i = 0; i < bit; i++) {
		bin1[i] = bin[i]; //assign number of bits pushed out of array to new array
	}
	for (uint32_t i = 0; i < SIZE - bit; i++) {
		bin[i] = bin[i + bit]; //move all bit to the left
	}
	for (uint32_t i = 0; i < bit; i++) {
		bin[SIZE - bit + i] = bin1[i]; //push back bits that are stored in new array
	}
	return *this = BinToDec(bin);
}
QInt QInt::operatorror(uint32_t bit) 
{
	bool *bin1 = new bool[bit];
	bool *bin = this->DecToBin(); //convert into binary
	for (uint32_t i = 0; i < bit; i++) {
		bin1[i] = bin[SIZE - 1 - i]; //assign number of bits pushed out of array to new array
	}
	for (uint32_t i = SIZE - 1; i >= bit; i--) {
		bin[i] = bin[i - bit]; //move all bit to the right
	}
	for (uint32_t i = 0; i < bit; i++) {
		bin[bit - 1 - i] = bin1[i]; //push back bits that are stored in new array
	}
	return *this = BinToDec(bin);
}

vector<string> ReadFromFile(string filename)
{
	int i = 0;
	vector<string> temp;
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open())
	{
		return {};
	}
	while (fin)
	{
		string a;
		getline(fin, a, '\n');
		temp.push_back(a);
		i++;
	}
	fin.close();
	return temp;
}
unsigned int QInt::getModeFromString(string a)
{
	string space = " ";
	int pos = a.find(space);
	pos--;
	string temp;
	for (int i = 0; i <= pos; i++)
	{
		temp[i] = a[i];
	}
	if (temp.compare("2") == 0)
	{
		return 0;
	}
	if (temp.compare("10") == 0)
	{
		return 1;
	}
	return 2;
}

bool* BinaryStringToBinaryBit(string s)
{
	bool * bit = new bool[SIZE];
	for (unsigned i = 0;i<SIZE;i++)
	{
		if (s[s.length() - i -1] == '1')
			bit[i] = 1;
		else
			bit[i] = 0;
	}
	return bit;
}
string getFourBitOfHex(int y)
{
	string a[16] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
	return a[y];
}

QInt hexStrToQInt(string s)
{
	QInt res;
	string str_bin = "";
	int len = s.length();
	for (int i = 0; i < len; i++)
	{
		int temp;
		if (s[i] <= '9')
		{
			temp = s[i] - '0';
		}
		else
		{
			temp = s[i] - 55;
		}
		str_bin += getFourBitOfHex(temp);
	}
	while (str_bin.length() < 128)
	{
		str_bin = '0' + str_bin;
	}
	bool *bit = new bool[128];
	for (int i = 0; i < 128; i++)
	{
		bit[i] = str_bin[i] - '0';
	}
	QInt temp;
	temp = QInt::BinToDec(bit);
	delete[] bit;
	return temp;
}

QInt caculateInQInt(QInt x, QInt y, string _operator)
{
	if (_operator == "+")
		return x + y;
	else if (_operator == "-")
		return x - y;
	else if (_operator == "*")
		return x * y;
	else if (_operator == "/")
		return x / y;
	else if (_operator == "&")
		return x & y;
	else if (_operator == "|")
		return x | y;
	else if (_operator == "^")
		return x ^ y;
	else if (_operator == "~")
		return ~x;
	else if (_operator == ">>")
		return x.ArithmeticShiftLeft(x,y);

	return x.ArithmeticShiftRight(x, y);
}

void numberCalculator(string a, string b, string c, string d)
{
	QInt x, y;
	if (a == "2")
	{
		bool *bit = BinaryStringToBinaryBit(b);
		x = QInt::BinToDec(bit);
		delete[]bit;
		if (c == "rol" || c == "ror")
		{
			int y = stoi(d);
			if (c == "ror")	 x = x.operatorror(y);
			else x = x.operatorrol(y);
			bit = x.DecToBin();
			for (int i = 0; i < SIZE; i++)
				cout << bit[i];
			cout << endl;
			return;
		}
		else
		{
			bit = BinaryStringToBinaryBit(d);
			x = QInt::BinToDec(bit);
			delete[]bit;
			caculateInQInt(x, y, c).PrintQInt();
		}

	}
	else if (a == "10")
	{
		x.ScanQInt(b);
		if (c == "rol"|| c== "ror")
		{
			int y = stoi(d);
			if (c == "ror")	 x = x.operatorror(y);
			else x = x.operatorrol(y);
			x.PrintQInt();
			return;
		}
		else
		{
			y.ScanQInt(d);
			caculateInQInt(x, y, c).PrintQInt();
		}
	}
	else if (a == "16")
	{
		x = hexStrToQInt(b);
		if (c == "rol" || c == "ror")
		{
			int y = stoi(d);
			if (c == "ror")	 x = x.operatorror(y);
			else x = x.operatorrol(y);
			string res = x.DecToHex();
			cout << res;
			return;
		}
		else
		{
			y = hexStrToQInt(d);
			string res;
			res = caculateInQInt(x, y, c).DecToHex();
			cout << res;
		}
	}
}

void numberConversion(string a, string b, string s)
{
	QInt x;
	string res;

	if (a == "2") {
		bool *bit = BinaryStringToBinaryBit(b);
		x = QInt::BinToDec(bit);
		delete[]bit;
		if (b == "~")
		{
			QInt y = ~x;
			y.PrintQInt();
			return;
		}
		if (b == "10") {
			x.PrintQInt();
		}
		else {
			cout << x.DecToHex();
		}
	}
	else if (a == "10")
	{
		x.ScanQInt(s);
		if (b == "~")
		{
			QInt y = ~x;
			y.PrintQInt();
			return;
		}
		if (b == "2") {
			bool *bit = x.DecToBin();
			for (int i = 0; i < SIZE; i++)
			{
				if (bit[i])
					cout << "1";
				else cout << "0";
				delete[]bit;
				return;
			}
		}
		else {
			cout << x.DecToHex();
		}
	}
	else
	{
		if (b == "~")
		{
			QInt y = ~hexStrToQInt(s);
			y.PrintQInt();
			return;
		}
		if (b == "2")
		{
			bool *bit = hexStrToQInt(s).DecToBin();
			for (int i = 0; i < SIZE; i++)
			{
				if (bit[i])
					cout << "1";
				else cout << "0";
				delete[]bit;
				return;
			}
			return;
		}
		else
		{
			hexStrToQInt(s).PrintQInt();
		}
	}
}

void ExcuteQInt(string input, string output)
{
	freopen(input.c_str(), "r", stdin);
	freopen(output.c_str(), "w", stdout);

	string s;
	while (getline(cin, s))
	{
		stringstream ss(s);
		vector<string> arr;
		while (ss >> s)
		{
			arr.push_back(s);
		}
		if (arr.size() == 3)
		{
			numberConversion(arr[0], arr[1], arr[2]);
		}
		else
		{
			numberCalculator(arr[0], arr[1], arr[2], arr[3]);
		}
	}
	fclose(stdin);
	fclose(stdout);
}

void ExcuteQInt()
{
	string s;
	while (getline(cin, s))
	{
		stringstream ss(s);
		vector<string> arr;
		while (ss >> s)
		{
			arr.push_back(s);
		}
		if (arr.size() == 3)
		{
			numberConversion(arr[0], arr[1], arr[2]);
		}
		else
		{
			numberCalculator(arr[0], arr[1], arr[2], arr[3]);
		}
	}
}