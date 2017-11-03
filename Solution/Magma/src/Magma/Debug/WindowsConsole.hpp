#pragma once

#include "Console.hpp"

#include <thread>
#include <atomic>

namespace Magma
{
#ifndef MAGMA_IS_WINDOWS
	using WindowsConsole = NullConsole;
#else
	class WindowsConsole : public Console
	{
	private:
		friend Console;

		WindowsConsole();
		virtual ~WindowsConsole() final;

		// Inherited via Console
		virtual void DPrint(const std::string & text) final;
		virtual void DPrintLn(const std::string & text) final;
		virtual void DError(const std::string& text) final;
		virtual void DClear() final;
		void Send(const std::string& command);

		void InputThread();

		std::thread m_inputThread;

		void* m_sendPipe;
		void* m_showPipe;
		void* m_consoleProcess;

		void* m_startupInfo;
		void* m_processInfo;
	};
#endif
}