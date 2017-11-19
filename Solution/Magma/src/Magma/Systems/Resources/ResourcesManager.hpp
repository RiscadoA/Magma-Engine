#pragma once

#include "Resource.hpp"
#include "..\MessageBus.hpp"

#include <map>

namespace Magma
{
	/// <summary>
	///		Manages the engine's resources, such as models, audio and textures.
	/// </summary>
	class ResourcesManager : public MessageListener
	{
	public:
		/// <summary>
		///		Loads all resources info from a folder
		/// </summary>
		/// <param name="resourcesFolder">Path to resources folder</param>
		void LoadInfo(const std::string& resourcesFolder);

		/// <summary>
		///		Gets a resource by its name (loads it if not loaded already)
		/// </summary>
		/// <param name="name">Resource name</param>
		/// <returns>Resource</returns>
		std::shared_ptr<Resource> Get(const std::string& name);

	private:
		void LoadSingleInfo(const std::string& path);

		std::map<std::string, ResourceInfo> m_resourcesInfo;

		// Inherited via MessageListener
		virtual void DerivedInit() override;
		virtual void DerivedTerminate() override;
	};
}