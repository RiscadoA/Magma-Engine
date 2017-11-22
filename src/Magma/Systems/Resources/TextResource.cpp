#include "TextResource.hpp"

#include <fstream>

void Magma::TextResource::Load(const ResourceInfo & resourceInfo)
{
	this->Unload();

	std::ifstream ifs(resourceInfo.GetPath());
	if (!ifs.is_open())
	{
		MAGMA_ERROR("Couldn't load text resource, file \"" + resourceInfo.GetPath() + "\" failed to open");
		return;
	}

	std::string line;
	while (std::getline(ifs, line))
	{
		m_text += line + "\n";
	}
	m_text.pop_back();
}

void Magma::TextResource::Unload()
{
	m_text.clear();
	m_text.shrink_to_fit();
}
