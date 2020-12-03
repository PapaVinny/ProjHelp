#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "parseRun.h"

int main(int argc, char* argv[])
{
	std::ifstream stream;
	std::ifstream* file = &stream;
	const char* filename = "";

	if (argc == 2)
	{
		filename = argv[1];
		file->open(filename);

		if (!file->is_open())
		{
			std::cout << "CANNOT OPEN THE FILE " << filename << std::endl;
			return -1;
		}
	}
	else if (argc > 2)
	{
		std::cout << "ONLY ONE FILE NAME ALLOWED" << std::endl;
		return -1;
	}

	int line = 0;
	bool success = Prog(*file, line);

	if (success)
	{
		std::cout << "Successful Execution" << std::endl;
	}
	else
	{
		std::cout << "Unsuccessful Interpretation" << std::endl;
		std::cout << "Number of Syntax Errors: " << error_count << std::endl;
	}

	file->close();
	return 0;
}