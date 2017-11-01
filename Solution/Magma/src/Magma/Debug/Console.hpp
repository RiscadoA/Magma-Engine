#pragma once

#include <string>

namespace Magma
{
	/// <summary>
	///		Interface used to create a layer of abstraction between the multiple types of consoles.
	///		Default console type is NullConsole.
	/// </summary>
	class Console
	{
	public:
		/// <summary>
		///		Sets the current console type
		/// </summary>
		template <typename T>
		static void Set();

		/// <summary>
		///		Prints something to the console
		/// </summary>
		/// <param name="out">Console output string</param>
		static void Print(const std::string& out);

		/// <summary>
		///		Prints a whole line to the console
		/// </summary>
		/// <param name="out">Console output string</param>
		static void PrintLn(const std::string& out);

		/// <summary>
		///		Reads something from the console
		/// </summary>
		/// <param name="in">Console input string</param>
		static void Read(std::string& in);

		/// <summary>
		///		Reads a whole line from the console
		/// </summary>
		/// <param name="in">Console input string</param>
		static void ReadLn(std::string& in);

	protected:
		Console() = default;
		virtual ~Console() = default;

		virtual void DPrint(const std::string& out) = 0;
		virtual void DPrintLn(const std::string& out) = 0;
		virtual void DRead(std::string& in) = 0;
		virtual void DReadLn(std::string& in) = 0;

	private:
		static Console* s_activeConsole;
	};

	/// <summary>
	///		Null Console implementation
	/// </summary>
	class NullConsole final : public Console
	{
		inline virtual void DPrint(const std::string& out) final {};
		inline virtual void DPrintLn(const std::string& out) final {};
		inline virtual void DRead(std::string& in) final {};
		inline virtual void DReadLn(std::string& in) final {};
	};

	/// <summary>
	///		Standard Console implementation (using C++ standard library)
	/// </summary>
	class STDConsole final : public Console
	{
		virtual void DPrint(const std::string& out) final;
		virtual void DPrintLn(const std::string& out) final;
		virtual void DRead(std::string& in) final;
		virtual void DReadLn(std::string& in) final;
	};

	template<typename T>
	inline void Console::Set()
	{
		if (s_activeConsole != nullptr)
			delete s_activeConsole;
		s_activeConsole = new T();
	}
}