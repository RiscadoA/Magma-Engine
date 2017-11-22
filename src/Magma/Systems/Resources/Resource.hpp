#pragma once

#include "..\..\Utils\INIDocument.hpp"
#include "..\..\Utils\Registrable.hpp"

#include <memory>

namespace Magma
{
	class Resource;

	/// <summary>
	///		Stores info about a resource. Loaded on engine startup
	/// </summary>
	class ResourceInfo : public Serializable
	{
	public:
		ResourceInfo(const std::string& currentFolder = "");

		/// <summary>
		///		Sets the resource that owns this info
		/// </summary>
		/// <param name="resource">Resource</param>
		inline void SetResource(std::weak_ptr<Resource> resource) { m_resource = resource; }

		/// <summary>
		///		Gets the resource that owns this info
		/// </summary>
		/// <returns>Resource if loaded, otherwise nullptr</returns>
		inline std::shared_ptr<Resource> GetResource() { return m_resource.lock(); }

		/// <summary>
		///		Gets the resource name
		/// </summary>
		/// <returns>The resource name</returns>
		inline const std::string& GetName() const { return m_iniDocument.GetSection("General").GetKey("Name"); }

		/// <summary>
		///		Gets the resource type
		/// </summary>
		/// <returns>This resource type</returns>
		inline const std::string& GetType() const { return m_iniDocument.GetSection("General").GetKey("Type"); }

		/// <summary>
		///		Gets the resource path
		/// </summary>
		/// <returns>This resource path</returns>
		inline std::string GetPath() const { return ((m_currentFolder == "") ? "" : (m_currentFolder)) + m_iniDocument.GetSection("General").GetKey("Path"); }

		/// <summary>
		///		Gets a resource info variable
		/// </summary>
		/// <param name="name">Variable name</param>
		/// <returns>Variable value</returns>
		inline const std::string& GetInfo(const std::string& name) const { return m_iniDocument.GetSection("Info").GetKey(name); }

		// Inherited from Serializable
		void Serialize(std::ostream& stream) const final;
		void Deserialize(std::istream& stream) final;

	private:
		std::weak_ptr<Resource> m_resource;
		INIDocument m_iniDocument;
		std::string m_currentFolder;
	};

	/// <summary>
	///		Class used to store resource data
	/// </summary>
	class Resource : public Registrable<Resource>
	{
	public:
		/// <summary>
		///		Casts this resource to another resource type (uses dynamic_cast).
		///		Calls MAGMA_ERROR when cast fails.
		/// </summary>
		/// <returns>Cast resource</returns>
		template <typename T> T& As()
		{
			T* rsc = dynamic_cast<T*>(this);
			if (rsc == nullptr)
				MAGMA_ERROR("Failed to cast resource to derived type");
			return *rsc;
		}

	private:
		friend class ResourcesManager;

		/// <summary>
		///		Loads this resource from its resource info
		/// </summary>
		/// <param name="resourceInfo"></param>
		virtual void Load(const ResourceInfo& resourceInfo) = 0;

		/// <summary>
		///		Unloads this resource
		/// </summary>
		virtual void Unload() = 0;
	};
}