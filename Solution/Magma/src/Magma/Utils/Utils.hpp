#pragma once

#include <string>

namespace Magma
{
	/// <summary>
	///		Shows an error message and aborts the program. You should use the macro MAGMA_ERROR to automatically fill in the file and line arguments
	/// </summary>
	/// <param name="error">The error message to be shown</param>
	/// <param name="file">The file where the error occured</param>
	/// <param name="line">The line where the error occured</param>
	void Error(const std::string& error, const std::string& file = "Unknown", size_t line = 0);
}

/// <summary>
///		Shows an error message and aborts the program.
/// </summary>
#define MAGMA_ERROR(error) do { Magma::Error(error, __FILE__, __LINE__); } while(false)

#if defined(_WIN64)
#define MAGMA_IS_WINDOWS
#define MAGMA_IS_X64
#elif defined(_WIN32)
#define MAGMA_IS_WINDOWS
#define MAGMA_IS_X86
#endif