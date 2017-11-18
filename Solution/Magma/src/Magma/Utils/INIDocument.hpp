#pragma once

#include "Serializable.hpp"

#include <map>

namespace Magma
{
	/// <summary>
	///		Class used to read from and write to INI document files
	/// </summary>
	class INIDocument final : public Serializable
	{
	public:
		/// <summary>
		///		Class used to represent a INI document section
		/// </summary>
		class Section
		{
		public:
			/// <summary>
			///		Sets the value of a key
			/// </summary>
			/// <param name="keyName">Key's name</param>
			/// <param name="value">Key's value</param>
			inline void SetKey(const std::string& keyName, const std::string& value) { m_keys[keyName] = value; }

			/// <summary>
			///		Removes a key
			/// </summary>
			/// <param name="keyName">Key's name</param>
			inline void RemoveKey(const std::string& keyName) { m_keys.erase(keyName); }

			/// <summary>
			///		Returns the value of a key
			/// </summary>
			/// <param name="keyName">Key's name</param>
			/// <returns>Key's value</returns>
			inline const std::string& GetKey(const std::string& keyName) const { return m_keys.at(keyName); }

			/// <summary>
			///		Returns every key in this section
			/// </summary>
			/// <returns>Every key in this section</returns>
			inline const std::map<std::string, std::string>& GetKeys() const { return m_keys; }

		private:
			std::map<std::string, std::string> m_keys;
		};

		/// <summary>
		///		Removes a section from this document
		/// </summary>
		/// <param name="name">Section's name</param>
		inline void RemoveSection(const std::string& name) { m_sections.erase(name); }

		/// <summary>
		///		Sets a section
		/// </summary>
		/// <param name="name">Section's name</param>
		/// <param name="section">New section</param>
		inline void SetSection(const std::string& name, const Section& section) { m_sections[name] = section; }

		/// <summary>
		///		Returns a reference to a section
		/// </summary>
		/// <param name="name">Section's name</param>
		/// <returns>Reference to section found</returns>
		inline Section& GetSection(const std::string& name) { return m_sections[name]; }

	private:
		std::map<std::string, Section> m_sections;

		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const override;
		virtual void Deserialize(std::istream & stream) override;
	};
}