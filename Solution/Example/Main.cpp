#include <iostream>

#include <Magma\Debug\WindowsConsole.hpp>
#include <Magma\Math\Matrix.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	Console::Init<WindowsConsole>();

	Matrix2x2 mat;

	std::cout << mat << std::endl;

	while (true)
	{
		std::string str;
		std::getline(std::cin, str);
		if (str == "exit")
			break;
		else
			MAGMA_WARNING("Unknown command \"" + str + "\"");
		std::cout << "Received \"" << str << "\"" << std::endl;
	}

	return 0;
}