#define _CRT_SECURE_NO_WARNINGS
#include "QInt.h"
#include "QFloat.h"
#include <iostream>
#include <string>
using namespace std;


int main(int argc, char *argv[])
{
	if (argc == 4)
	{
		string input(argv[1]);
		string output(argv[2]);

		if (argv[3] == "1")
		{
			ExcuteQInt(input, output);
			return 0;

		}

		if (argv[3] == "2")
		{
			ExecuteQFloat(input, output);
			return 0;
		}

	}

	


	system("pause");
	return 0;
}