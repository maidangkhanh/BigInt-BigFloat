#define _CRT_SECURE_NO_WARNINGS
#include "QInt.h"
#include "QFloat.h"
#include <iostream>
#include <string>
using namespace std;


int main(int argc, char *argv[])
{
	if (argc == 3)
	{
		string input(argv[0]);
		string output(argv[1]);

		if (argv[2] == "1")
			ExcuteQInt(input, output);

		if (argv[2] == "2")
			ExecuteQfloat(input, output);

	}

	return 0;
}