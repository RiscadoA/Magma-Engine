#include <iostream>

#include <Magma\Debug\WindowsConsole.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	Console::Init<WindowsConsole>();

	while (true)
	{
		std::string str;
		std::getline(std::cin, str);
		if (str == "exit")
			break;
		else
			MAGMA_ERROR("Unknown command \"" + str + "\"");
		std::cout << "\"" << str << "\"" << std::endl;
	}

	return 0;
}