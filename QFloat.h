#pragma once
#include <iostream>

class QFloat {
public:
	QFloat();
	QFloat(std::string);
	void ScanQFloat();
	void ScanQFloat(std::string);
	void PrintQFloat();
	QFloat BinToDec(bool *bin); 
	bool* DecToBin() const;
	bool isZero();
	bool isINF();
	bool isNaN();
	bool isDenormalisedNum();
	bool isSpecialNum();
private:
	uint32_t data[4];
};

void ExecuteQFloat(std::string,std::string);
void ExcuteQFloat();