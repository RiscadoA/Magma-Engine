#include "Globals.hpp"

std::map<std::string, Magma::GlobalVar> Magma::Globals::s_globals = {};
std::mutex Magma::Globals::s_mutex;

void Magma::GlobalVar::Serialize(std::ostream & stream) const
{
	stream << m_value << '\\' << '#';
}

void Magma::GlobalVar::Deserialize(std::istream & stream)
{
	m_value = "";
	while (stream.peek() != std::istream::traits_type::eof())
	{
		auto now = stream.get();
		auto next = stream.peek();

		if (now == '\\' && next == '#')
			break;
		else
			m_value += now;
	}
}

Magma::GlobalVar & Magma::Globals::Get(const std::string & varName)
{
	std::lock_guard<std::mutex> lockGuard(s_mutex);
	return s_globals[varName];
}
