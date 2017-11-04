#include <iostream>

#include <Magma\Debug\WindowsConsole.hpp>
#include <Magma\Utils\Math.hpp>

#include <algorithm>

using namespace Magma;

int main(int argc, char** argv)
{
	Console::Init<WindowsConsole>();

	while (true)
	{
		std::string str;
		std::getline(std::cin, str);
		if (str.empty())
			continue;
		if (str == "exit")
			break;
		else
			MAGMA_WARNING("Unknown command \"" + str + "\"");
		std::cout << "Received \"" << str << "\"" << std::endl;
	}

	return 0;
}