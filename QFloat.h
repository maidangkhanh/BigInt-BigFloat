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
private:
	uint32_t data[4];
};
