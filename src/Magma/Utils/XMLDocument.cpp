#include "XMLDocument.hpp"
#include "Utils.hpp"

#include <functional>
#include <sstream>

static void MultipleDelimsGetline(std::istream & stream, std::string& line, const std::string& delims)
{
	std::getline(stream, line, delims[0]);
	for (size_t i = 1; i < delims.size(); ++i)
	{
		std::stringstream ss(line);
		std::getline(ss, line, delims[i]);
	}
}

Magma::XMLDocument::XMLDocument()
{
	// Create root element
	m_root = new Element();
	m_root->m_name = "root";
}

Magma::XMLDocument::~XMLDocument()
{
	// Delete root element
	delete m_root;
}

void Magma::XMLDocument::Serialize(std::ostream & stream) const
{
	static const std::function<bool(const std::string&)> isTextInline = [](const std::string& text) -> bool
	{
		for (auto& c : text)
			if (c == '\n')
				return false;
		return true;
	};

	static const std::function<std::string(const std::string&, uint32_t)> formatMultiline = [](const std::string& text, uint32_t indentation) -> std::string
	{
		std::string n = "";
		for (uint32_t ind = 0; ind < indentation; ++ind) n += '\t';
		for (const auto& c : text)
		{
			n += c;
			if (c == '\n')
				for (uint32_t ind = 0; ind < indentation; ++ind) n += '\t';
		}
		if (n.size() > 0 && n[n.size() - 1] == '\n')
			n.erase(n.begin() + (n.size() - 1));
		return n;
	};

	static const std::function<void(std::ostream&, Element*, uint32_t)> recursiveSerialize = [](std::ostream & stream, Element* element, uint32_t indentation)
	{
		// Set indentation
		for (uint32_t ind = 0; ind < indentation; ++ind)
			stream << "\t";
		// Begin open tag
		stream << "<" << element->GetName();
		// Add attributes
		const auto& attributes = element->GetAttributes();
		for (const auto& attrib : attributes)
			stream << " " << attrib.first << "=\"" << attrib.second << "\"";
		// End open tag
		stream << ">";

		bool isInline = true;

		// Add element text
		if (isTextInline(element->GetText()))
			stream << element->GetText();
		else
		{
			stream << std::endl << formatMultiline(element->GetText(), indentation + 1) << std::endl;
			isInline = false;
		}

		// If this element has children, Recursively serialize them
		const auto& children = element->GetChildren();
		if (children.size() > 0)
		{
			if (isInline)
				stream << std::endl;
			isInline = false;
			for (auto& child : children)
			{
				recursiveSerialize(stream, child, indentation + 1);
				stream << std::endl;
			}
		}

		// Set indentation
		if (!isInline)
			for (uint32_t ind = 0; ind < indentation; ++ind)
				stream << "\t";
		// Close tag
		stream << "</" << element->GetName() << ">";
	};

	// Recursively serialize the elements in this XML Document
	recursiveSerialize(stream, m_root, 0);
}

void Magma::XMLDocument::Deserialize(std::istream & stream)
{
	delete m_root;
	m_root = nullptr;

	bool end = false;

	Element* current = nullptr;
	uint32_t indentation = 0;
	uint32_t remainingIndentation = 0;

	do
	{
		if (current == nullptr && (stream.peek() == '\t' || stream.peek() == L' ' || stream.peek() == '\n'))
		{
			stream.get();
			continue;
		}

		if (current != nullptr && stream.peek() != '<')
		{
			char character = stream.get();

			// Only add tabs that are not from the indentation to the text
			if (character == '\t')
			{
				if (remainingIndentation == 0)
				{
					if (current->m_text != "")
						current->m_text += '\t';
				}
				else
					--remainingIndentation;
			}
			// Add characters to text
			else
			{
				if (character == '\n')
					remainingIndentation = indentation;
				if (!((character == '\n' && current->m_text == "") || (character == ' ' && current->m_text == "")))
					current->m_text += std::string("") + character;
			}

			/*ACE_WERROR(Wstd::string(L"Failed to deserialize XML Document, invalid character found '") + static_cast<WCHR>(stream.peek()) + L"'");
			return;*/
		}
		else
		{
			stream.get();
			switch (stream.peek())
			{
			case '/': // Closing tag
				{
					stream.get();
					std::string name;
					std::getline(stream, name, '>');

					if (current == nullptr)
					{
						MAGMA_ERROR("Failed to deserialize XML Document, couldn't close tag because there is no tag to close (\"" + name + "\")");
						return;
					}

					if (name != current->m_name)
					{
						MAGMA_ERROR("Failed to deserialize XML Document, couldn't close tag because names do not match: \"" + name + "\" & \"" + current->m_name + "\"");
						return;
					}

					if (current->m_text.size() > 0 && current->m_text[current->m_text.size() - 1] == '\n')
						current->m_text.erase(current->m_text.begin() + current->m_text.size() - 1);

					--indentation;
					current = current->m_parent;
					if (current == nullptr)
						end = true;
				}
				break;
			default: // Opening tag
				{
					std::string content;
					std::getline(stream, content, '>');
					std::stringstream contentSS(content);

					// Get name
					std::string name;
					std::getline(contentSS, name, ' ');
					
					if (current == nullptr)
					{
						m_root = new Element();
						current = m_root;
						current->m_parent = nullptr;
						current->SetName(name);
					}
					else current = current->AddChild(name);
					current->m_text = "";

					// Get attributes
					std::string attribute;
					while (std::getline(contentSS, attribute,' '))
					{
						std::stringstream attributeSS(attribute);
						std::string attributeName, attributeValue;
						std::getline(attributeSS, attributeName, '=');
						attributeSS.ignore(64, '"');
						std::getline(attributeSS, attributeValue, '"');
						current->SetAttribute(attributeName, attributeValue);
					}

					++indentation;
					remainingIndentation = indentation;
				}
				break;
			}
		}
		
	} while (!end);
}

Magma::XMLDocument::Element * Magma::XMLDocument::Element::GetChild(const std::string & name)
{
	for (auto& c : m_children)
		if (c->m_name == name)
			return c;
	return nullptr;
}

std::list<Magma::XMLDocument::Element*> Magma::XMLDocument::Element::GetChildren(const std::string & name)
{
	std::list<Magma::XMLDocument::Element*> children;
	for (auto& c : m_children)
		if (c->m_name == name)
			children.push_back(c);
	return children;
}

bool Magma::XMLDocument::Element::RemoveChild(const std::string & name)
{
	for (auto it = m_children.begin(); it != m_children.end();)
		if ((*it)->m_name == name)
		{
			delete *it;
			m_children.erase(it);
			return true;
		}
	return false;
}

bool Magma::XMLDocument::Element::RemoveChildren(const std::string & name)
{
	bool removedAny = false;
	for (auto it = m_children.begin(); it != m_children.end();)
	{
		auto oldIt = it;
		++it;
		if ((*oldIt)->m_name == name)
		{
			removedAny = true;
			delete *oldIt;
			m_children.erase(oldIt);
		}
	}
	return removedAny;
}

Magma::XMLDocument::Element * Magma::XMLDocument::Element::AddChild(const std::string & name)
{
	auto element = new Element();
	element->m_name = name;
	element->m_attributes = {};
	element->m_children = {};
	element->m_text = "";
	element->m_parent = this;

	m_children.push_back(element);
	return element;
}

Magma::XMLDocument::Element::~Element()
{
	// Delete child elements
	for (auto c : m_children)
		delete c;
}
