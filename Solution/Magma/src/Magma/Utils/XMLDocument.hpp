#pragma once

#include "Serializable.hpp"

#include <map>
#include <list>

namespace Magma
{
	/// <summary>
	///		Class used to read from and write to XML document files
	/// </summary>
	class XMLDocument final : public Serializable
	{
	public:
		XMLDocument();
		~XMLDocument();

		/// <summary>
		///		XML Document element
		/// </summary>
		class Element final
		{
		public:
			/// <summary>
			///		Returns an attribute of this element
			/// </summary>
			/// <param name="name">Attribute name</param>
			/// <returns>Attribute value</returns>
			inline const std::string& GetAttribute(const std::string& name) { return m_attributes[name]; }

			/// <summary>
			///		Returns every attribute this element has
			/// </summary>
			/// <returns>Every attribute present in this element</returns>
			inline const auto& GetAttributes() { return m_attributes; }

			/// <summary>
			///		Sets an attribute of this element
			/// </summary>
			/// <param name="name">Attribute name</param>
			/// <param name="value">New attribute value</param>
			inline void SetAttribute(const std::string& name, const std::string& value) { m_attributes[name] = value; }

			/// <summary>
			///		Removes an attribute from this element
			/// </summary>
			/// <param name="name">Attribute name</param>
			inline void RemoveAttribute(const std::string& name) { m_attributes.erase(name); }

			/// <summary>
			///		Returns the first child found of this element with the right name
			/// </summary>
			/// <param name="name">Child name</param>
			/// <returns>First child found with name, nullptr if none</returns>
			Element* GetChild(const std::string& name);

			/// <summary>
			///		Returns every child found of this element with the right name
			/// </summary>
			/// <param name="name">Children name</param>
			/// <returns>Every child found with name</returns>
			std::list<Element*> GetChildren(const std::string& name);

			/// <summary>
			///		Returns every child in this element
			/// </summary>
			/// <returns>Every child in this element</returns>
			inline const std::list<Element*>& GetChildren() { return m_children; }

			/// <summary>
			///		Removes the first child found with name
			/// </summary>
			/// <param name="name">Child's name</param>
			/// <returns>True if child was removed, otherwise false</returns>
			bool RemoveChild(const std::string& name);

			/// <summary>
			///		Removes every child found with name
			/// </summary>
			/// <param name="name">Children name</param>
			/// <returns>True if any child was removed, otherwise false</returns>
			bool RemoveChildren(const std::string& name);

			/// <summary>
			///		Adds a child with the chosen name and returns it
			/// </summary>
			/// <returns>New child</returns>
			Element* AddChild(const std::string& name);

			/// <summary>
			///		Returns the text present in this element
			/// </summary>
			/// <returns>Text present in this element</returns>
			inline const std::string& GetText() { return m_text; }

			/// <summary>
			///		Sets the text present in this element
			/// </summary>
			/// <param name="text">New text</param>
			inline void SetText(const std::string& text) { m_text = text; }

			/// <summary>
			///		Returns this element's name
			/// </summary>
			/// <returns>This element's name</returns>
			inline const std::string& GetName() { return m_name; }

			/// <summary>
			///		Sets this element's name
			/// </summary>
			/// <param name="name">New element name</param>
			inline void SetName(const std::string& name) { m_name = name; }

			/// <summary>
			///		Returns this element's parent
			/// </summary>
			inline Element* GetParent() { return m_parent; }

		private:
			Element() = default;
			~Element();

			friend XMLDocument;

			std::string m_text;
			std::string m_name;
			std::map<std::string, std::string> m_attributes;
			std::list<Element*> m_children;
			Element* m_parent = nullptr;
		};

		/// <summary>
		///		Returns this element's root element
		/// </summary>
		/// <returns>This element's root element</returns>
		inline Element* GetRoot() { return m_root; }

	private:
		Element* m_root;

		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const override;
		virtual void Deserialize(std::istream & stream) override;
	};
}
