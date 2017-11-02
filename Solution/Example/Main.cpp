#include <iostream>

#include <Magma\Debug\WindowsConsole.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	Console::Init<WindowsConsole>();

	std::cout << "AA" << std::endl;

	MAGMA_ERROR("Ai!");

	while (true)
	{
		std::string str = Console::Read();
		if (str == "exit")
			break;
		Console::PrintLn(str);
	}

	return 0;
}