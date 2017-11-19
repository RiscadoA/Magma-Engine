#pragma once

#include "Resource.hpp"

namespace Magma
{
	/// <summary>
	///		Class used to store a text resource
	/// </summary>
	class TextResource : public Resource
	{
	public:
		/// <summary>
		///		Gets the text loaded in this resource
		/// </summary>
		/// <returns>Text</returns>
		const std::string& Text() const { return m_text; }

	private:
		// Inherited via Resource
		virtual void Load(const ResourceInfo & resourceInfo) override;
		virtual void Unload() override;

		std::string m_text;
	};
	MAGMA_REGISTER(Resource, TextResource, "Text");
}