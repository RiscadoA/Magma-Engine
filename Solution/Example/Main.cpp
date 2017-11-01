#include <iostream>

#include <Magma\Debug\Console.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	Console::Set<STDConsole>();

	std::string name;

	Console::PrintLn("Enter your name: ");
	Console::ReadLn(name);

	Console::PrintLn("Hi " + name + "!");
	Console::Read(std::string());

	return 0;
}