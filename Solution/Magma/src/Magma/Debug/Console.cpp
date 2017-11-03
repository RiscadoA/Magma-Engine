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

class ofunctionbuf
	: public std::streambuf
{
private:
	typedef std::streambuf::traits_type traits_type;
	std::function<void(const std::string&)> d_function;
	char        d_buffer[1024];
	virtual int overflow(int c) override
	{
		if (!traits_type::eq_int_type(c, traits_type::eof()))
		{
			*this->pptr() = traits_type::to_char_type(c);
			this->pbump(1);
		}
		return this->sync() ? traits_type::not_eof(c) : traits_type::eof();
	}
	virtual int sync() override
	{
		if (this->pbase() != this->pptr())
		{
			this->d_function(std::string(this->pbase(), this->pptr()));
			this->setp(this->pbase(), this->epptr());
		}
		return 0;
	}
public:
	ofunctionbuf(std::function<void(const std::string&)> const& function)
		: d_function(function)
	{
		this->setp(this->d_buffer, this->d_buffer + sizeof(this->d_buffer) - 1);
	}
};

class ofunctionstream
	: private virtual ofunctionbuf
	, public std::ostream
{
public:
	ofunctionstream(std::function<void(const std::string&)> const& function)
		: ofunctionbuf(function)
		, std::ostream(static_cast<std::streambuf*>(this))
	{
		this->flags(std::ios_base::unitbuf);
	}
};

class ifunctionbuf
	: public std::streambuf
{
private:
	typedef std::streambuf::traits_type traits_type;
	std::function<void(std::string&)> d_function;
	char        d_buffer[1024];
	virtual int underflow() override
	{
		if (gptr() < egptr())
			return traits_type::to_int_type(*gptr());

		std::string str;
		d_function(str);
		if (str.empty())
			return traits_type::eof();
		
		std::memcpy(d_buffer, str.c_str(), str.size());
		if (d_buffer[str.size() - 1] != '\n')
		{
			d_buffer[str.size()] = ' ';
			setg(d_buffer, d_buffer, d_buffer + str.size() + 1);
		}
		else
		{
			setg(d_buffer, d_buffer, d_buffer + str.size());
		}

		return traits_type::to_int_type(*gptr());
	}
public:
	ifunctionbuf(std::function<void(std::string&)> const& function)
		: d_function(function)
	{
		this->setg(this->d_buffer, this->d_buffer, this->d_buffer);
	}
};

class ifunctionstream
	: private virtual ifunctionbuf
	, public std::istream
{
public:
	ifunctionstream(std::function<void(std::string&)> const& function)
		: ifunctionbuf(function)
		, std::istream(static_cast<std::streambuf*>(this))
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
	// Redirect std::cout, std::cerr and std::clog to console
	newOS = new ofunctionstream(Console::Print);
	oldBuf = std::cout.rdbuf();
	std::cout.rdbuf(newOS->rdbuf());

	newERROS = new ofunctionstream(Console::Error);
	oldBufERR = std::cerr.rdbuf();
	std::cerr.rdbuf(newERROS->rdbuf());

	newLOGOS = new ofunctionstream(Console::Warning);
	oldBufLOG = std::clog.rdbuf();
	std::clog.rdbuf(newLOGOS->rdbuf());

	// Redirect std::cin to console
	newIS = new ifunctionstream(Console::Read);
	oldIBuf = std::cin.rdbuf();
	std::cin.rdbuf(newIS->rdbuf());
}

Magma::Console::~Console()
{
	std::cout.rdbuf(oldBuf);
	std::cerr.rdbuf(oldBufERR);
	std::clog.rdbuf(oldBufLOG);
	std::cin.rdbuf(oldIBuf);

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
