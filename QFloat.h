#pragma once
#include <iostream>

class QFloat {
public:
	QFloat();
	QFloat(std::string);
	void ScanQFloat();
	void ScanQFloat(std::string);
	void printQFloat();
	QFloat BinToDec(bool *bin); 
	bool* DecToBin() const;
private:
	uint32_t data[4];
};
