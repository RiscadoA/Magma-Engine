#include "INIDocument.hpp"

#include "Utils.hpp"

#include <sstream>

void Magma::INIDocument::Serialize(std::ostream & stream) const
{
	stream << "@start";

	for (auto& s : m_sections)
	{
		stream << std::endl << "[" << s.first << "]" << std::endl;
		const auto& keys = s.second.GetKeys();
		for (const auto& k : keys)
			stream << k.first << "=" << k.second << std::endl;
	}

	stream << "@end";
}

void Magma::INIDocument::Deserialize(std::istream & stream)
{
	m_sections.clear();

	std::string line;
	Section* currentSection = nullptr;

	while (std::getline(stream, line))
	{
		if (line.empty() || line[0] == ';') continue;
		if (line == "@end") break;
		else if (line == "@start") continue;

		if (line[0] == '[')	// Start section
		{
			std::string name = line.substr(1, line.find_first_of(']') - 1);
			currentSection = &this->GetSection(name);
		}
		else // Add key
		{
			std::stringstream ss(line);

			std::string name, value;
			std::getline(ss, name, '=');
			std::getline(ss, value);

			if (currentSection == nullptr)
			{
				MAGMA_ERROR("Failed to deserialize INI Document, there was an attemp to add a key without setting a section first (key \"" + name + "=" + value + "\"");
				return;
			}
			currentSection->SetKey(name, value);
		}
	}
}
