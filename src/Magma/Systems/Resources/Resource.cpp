#include "Resource.hpp"

#include <sstream>

Magma::ResourceInfo::ResourceInfo(const std::string & currentFolder)
	: m_currentFolder(currentFolder)
{

}

void Magma::ResourceInfo::Serialize(std::ostream & stream) const
{
	stream << m_iniDocument;
}

void Magma::ResourceInfo::Deserialize(std::istream & stream)
{
	stream >> m_iniDocument;
}
