#include "Console.hpp"

#include <iostream>

Magma::Console* Magma::Console::s_activeConsole = new Magma::NullConsole();

void Magma::Console::Print(const std::string & out)
{
	s_activeConsole->DPrint(out);
}

void Magma::Console::PrintLn(const std::string & out)
{
	s_activeConsole->DPrintLn(out);
}

void Magma::Console::Read(std::string & in)
{
	s_activeConsole->DRead(in);
}

void Magma::Console::ReadLn(std::string & in)
{
	s_activeConsole->DReadLn(in);
}

void Magma::STDConsole::DPrint(const std::string & out)
{
	std::cout << out << std::flush;
}

void Magma::STDConsole::DPrintLn(const std::string & out)
{
	std::cout << out << std::endl;
}

void Magma::STDConsole::DRead(std::string & in)
{
	std::cin >> in;
}

void Magma::STDConsole::DReadLn(std::string & in)
{
	std::getline(std::cin, in);
}
