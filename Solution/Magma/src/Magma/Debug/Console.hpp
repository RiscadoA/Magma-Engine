#pragma once

#include "..\Utils\Utils.hpp"

#include <string>
#include <functional>
#include <map>
#include <mutex>
#include <atomic>
#include <sstream>

namespace Magma
{
	/// <summary>
	///		Class used as an interface to implement consoles
	/// </summary>
	class Console
	{
	public:
		template <typename T>
		static void Init() { delete s_activeConsole.m_console; s_activeConsole.m_console = new T(); }
		static void Terminate();

		inline static std::uint64_t AddInputCallback(std::function<void(const std::string&)> callback)
		{
			s_callbackMutex.lock(); s_callback[s_nextCallbackID++] = callback; s_callbackMutex.unlock();
			return s_nextCallbackID - 1;
		}

		inline static void RemoveInputCallback(std::uint64_t id)
		{
			s_callbackMutex.lock(); s_callback.erase(id); s_callbackMutex.unlock();
		}

		inline static void Print(const std::string& text) { s_activeConsole.m_console->DPrint(text); }
		inline static void PrintLn(const std::string& text) { s_activeConsole.m_console->DPrintLn(text); }
		inline static void Error(const std::string& text) { s_activeConsole.m_console->DError(text); }
		inline static void Warning(const std::string& text) { s_activeConsole.m_console->DWarning(text); }
		static void Read(std::string& text);
		inline static void Clear() { s_activeConsole.m_console->DClear(); }

	protected:
		Console();
		virtual ~Console();

		static std::map<std::uint64_t, std::function<void(const std::string&)>> s_callback;
		static std::uint64_t s_nextCallbackID;
		static std::mutex s_callbackMutex;

		virtual void DPrint(const std::string& text) = 0;
		virtual void DPrintLn(const std::string& text) = 0;
		virtual void DError(const std::string& text) = 0;
		virtual void DWarning(const std::string& text) = 0;
		virtual void DClear() = 0;

	private:
		static void InputCallback(const std::string& text);

		std::streambuf* oldBuf;
		std::ostream* newOS;
		std::streambuf* oldBufERR;
		std::ostream* newERROS;
		std::streambuf* oldBufLOG;
		std::ostream* newLOGOS;
		std::streambuf* oldIBuf;
		std::istream* newIS;

		static std::mutex s_readMutex;
		static std::atomic<bool> s_read;
		static std::atomic<bool> s_reading;
		static std::string s_textRead;

		class ConsoleHandle
		{
		public:
			ConsoleHandle(Console* console);
			~ConsoleHandle();

			ConsoleHandle& operator=(Console* console);

			Console* m_console;
		};

		static ConsoleHandle s_activeConsole;
	};

	/// <summary>
	///		Null implementation of Console interface
	/// </summary>
	class NullConsole : public Console
	{
	public:
		// Inherited via Console
		inline virtual void DPrint(const std::string & text) final {}
		inline virtual void DPrintLn(const std::string & text) final {}
		inline virtual void DError(const std::string& text) final {};
		inline virtual void DWarning(const std::string& text) final {};
		inline virtual void DClear() final {}
	};
}