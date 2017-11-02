#include "Console.hpp"

#include <iostream>

std::map<std::uint64_t, std::function<void(const std::string&)>> Magma::Console::s_callback = { { 0, Magma::Console::InputCallback } };
std::uint64_t Magma::Console::s_nextCallbackID = 1;
std::mutex Magma::Console::s_callbackMutex;
Magma::Console::ConsoleHandle Magma::Console::s_activeConsole = new NullConsole();
std::mutex Magma::Console::s_readMutex;
std::atomic<bool> Magma::Console::s_read = false;
std::atomic<bool> Magma::Console::s_reading = false;
std::string Magma::Console::s_textRead = "";

Magma::Console::ConsoleHandle::ConsoleHandle(Console * console)
{
	m_console = console;
}

Magma::Console::ConsoleHandle::~ConsoleHandle()
{
	delete m_console;
}

Magma::Console::ConsoleHandle& Magma::Console::ConsoleHandle::operator=(Console * console)
{
	delete m_console;
	m_console = console;
	return *this;
}

class functionbuf
	: public std::streambuf
{
private:
	typedef std::streambuf::traits_type traits_type;
	std::function<void(const std::string&)> d_function;
	char        d_buffer[1024];
	int overflow(int c)
	{
		if (!traits_type::eq_int_type(c, traits_type::eof()))
		{
			*this->pptr() = traits_type::to_char_type(c);
			this->pbump(1);
		}
		return this->sync() ? traits_type::not_eof(c) : traits_type::eof();
	}
	int sync()
	{
		if (this->pbase() != this->pptr())
		{
			this->d_function(std::string(this->pbase(), this->pptr()));
			this->setp(this->pbase(), this->epptr());
		}
		return 0;
	}
public:
	functionbuf(std::function<void(const std::string&)> const& function)
		: d_function(function)
	{
		this->setp(this->d_buffer, this->d_buffer + sizeof(this->d_buffer) - 1);
	}
};

class ofunctionstream
	: private virtual functionbuf
	, public std::ostream
{
public:
	ofunctionstream(std::function<void(const std::string&)> const& function)
		: functionbuf(function)
		, std::ostream(static_cast<std::streambuf*>(this))
	{
		this->flags(std::ios_base::unitbuf);
	}
};

void Magma::Console::Terminate()
{
	s_activeConsole = new NullConsole();
}

void Magma::Console::Read(std::string & text)
{
	s_readMutex.lock();
	s_textRead = "";
	s_read = false;
	s_reading = true;
	while (!s_read);
	text = s_textRead;
	s_reading = false;
	s_readMutex.unlock();
}

Magma::Console::Console()
{
	// Redirect std::cout and std::cerr to console
	newOS = new ofunctionstream(Console::Print);
	oldBuf = std::cout.rdbuf();
	std::cout.rdbuf(newOS->rdbuf());

	oldBufERR = std::cerr.rdbuf();
	std::cerr.rdbuf(newOS->rdbuf());
}

Magma::Console::~Console()
{
	std::cout.rdbuf(oldBuf);
	std::cerr.rdbuf(oldBufERR);

	delete newOS;
}

void Magma::Console::InputCallback(const std::string & text)
{
	if (s_reading)
	{
		s_textRead = text;
		s_read = true;
	}
}
