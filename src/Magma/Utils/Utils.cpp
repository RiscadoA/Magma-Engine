#include "Utils.hpp"

#include <iostream>

#ifdef MAGMA_IS_WINDOWS
#include <Windows.h>
void Magma::Error(const std::string & error, const std::string & file, size_t line)
{
	std::cerr << "Magma::Error on file \"" << file << "\", line " << line << ":" << std::endl << error << std::endl;
	auto answer = MessageBox(NULL, ("File \"" + file + "\" (" + std::to_string(line) + "); Message:\n" + error).c_str(), "Magma::Error, abort execution?", MB_YESNO | MB_ICONERROR);
	if (answer == IDYES)
		std::exit(1);
}
#else
void Magma::Error(const std::string & error, const std::string & file, size_t line)
{
	std::cerr << "Magma::Error on file \"" << file << "\", line " << line << ":" << std::endl << error << std::endl;
	std::abort();
}
#endif

void Magma::Warning(const std::string & warning, const std::string & file, size_t line)
{
	std::clog << "Magma::Warning on file \"" << file << "\", line " << line << ":" << std::endl << warning << std::endl;
}