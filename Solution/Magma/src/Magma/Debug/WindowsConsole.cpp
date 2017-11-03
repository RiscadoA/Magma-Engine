#include "WindowsConsole.hpp"

#include <Windows.h>

Magma::WindowsConsole::WindowsConsole()
{
	// Create Console process
	{
		m_startupInfo = new STARTUPINFO();
		m_processInfo = new PROCESS_INFORMATION();

		ZeroMemory(m_startupInfo, sizeof(((STARTUPINFO*)m_startupInfo)));
		((STARTUPINFO*)m_startupInfo)->cb = sizeof(((STARTUPINFO*)m_startupInfo));
		ZeroMemory(((PROCESS_INFORMATION*)m_processInfo), sizeof(*((PROCESS_INFORMATION*)m_processInfo)));

		if (!CreateProcess("Console.exe", // No module name (use command line)
						   "",            // Command line
						   NULL,           // Process handle not inheritable
						   NULL,           // Thread handle not inheritable
						   FALSE,          // Set handle inheritance to FALSE
						   0,              // No creation flags
						   NULL,           // Use parent's environment block
						   NULL,           // Use parent's starting directory 
						   ((STARTUPINFO*)m_startupInfo),            // Pointer to STARTUPINFO structure
						   ((PROCESS_INFORMATION*)m_processInfo))           // Pointer to PROCESS_INFORMATION structure
			)
		{
			throw std::runtime_error("[Windows Console] Failed to start Console process: " + GetLastError());
			return;
		}
	}

	// Initialize pipes
	m_showPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Magma_Console_Pipe_Read"),
								 PIPE_ACCESS_OUTBOUND,
								 PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
								 1,
								 1024 * 16,
								 1024 * 16,
								 NMPWAIT_USE_DEFAULT_WAIT,
								 NULL);

	if (m_showPipe == INVALID_HANDLE_VALUE)
	{
		throw std::runtime_error("[Windows Console] Failed to create named show pipe (INVALID_HANDLE_VALUE)");
	}
	else if (ConnectNamedPipe(m_showPipe, NULL) == FALSE)
	{
		throw std::runtime_error("[Windows Console] Failed to connect to named show pipe");
	}

	m_sendPipe = CreateFile(TEXT("\\\\.\\pipe\\Magma_Console_Pipe_Send"),
							GENERIC_READ,
							0,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);
	if (m_sendPipe == INVALID_HANDLE_VALUE)
		throw std::runtime_error("[Windows Console] Failed to create named send pipe (INVALID_HANDLE_VALUE)");

	m_inputThread = std::thread(&Magma::WindowsConsole::InputThread, this);
}

Magma::WindowsConsole::~WindowsConsole()
{
	// Send close message to console
	Send("close");

	// Terminate console process
	WaitForSingleObject(((PROCESS_INFORMATION*)m_processInfo)->hProcess, 1000); // Wait 1 second for the process to terminate
	TerminateProcess(((PROCESS_INFORMATION*)m_processInfo)->hProcess, 0); // Force process termination
	CloseHandle(((PROCESS_INFORMATION*)m_processInfo)->hProcess);
	CloseHandle(((PROCESS_INFORMATION*)m_processInfo)->hThread);

	// Disconnect from pipes
	DisconnectNamedPipe(m_showPipe);
	CloseHandle(m_sendPipe);
	m_inputThread.join();
}

void Magma::WindowsConsole::DPrint(const std::string & text)
{
	Send("print " + text);
}

void Magma::WindowsConsole::DPrintLn(const std::string & text)
{
	DPrint(text + "\n\r");
}

void Magma::WindowsConsole::DError(const std::string & text)
{
	Send("error " + text);
}

void Magma::WindowsConsole::DWarning(const std::string & text)
{
	Send("warng " + text);
}

void Magma::WindowsConsole::DClear()
{
	Send("clear");
}

void Magma::WindowsConsole::Send(const std::string & command)
{
	if (m_showPipe == INVALID_HANDLE_VALUE)
		return;

	DWORD dwWritten;

	// Send utf8 string to Console using the named pipe
	bool flg = WriteFile(m_showPipe,
						 command.c_str(),
						 command.length() + 1,
						 &dwWritten,
						 NULL);
	if (flg == false)
		throw std::runtime_error("[Windows Console] Failed to write to pipe");
}

void Magma::WindowsConsole::InputThread()
{
	while (m_sendPipe != INVALID_HANDLE_VALUE)
	{
		DWORD dwRead;

		char c[1026];

		bool flg = ReadFile(m_sendPipe,
							c,
							1024,
							&dwRead,
							NULL);

		if (flg == false)
			return;

		if (dwRead != 0)
		{
			if (c[dwRead - 1] != '\n')
			{
				c[dwRead] = '\n';
				c[dwRead + 1] = '\0';
			}
			else c[dwRead] = '\0';
			s_callbackMutex.lock();
			for (auto& callback : s_callback)
				callback.second(c);
			s_callbackMutex.unlock();
		}
	}
}
