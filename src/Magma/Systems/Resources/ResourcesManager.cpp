#include "ResourcesManager.hpp"

#include <filesystem>
#include <fstream>
#include <regex>

void Magma::ResourcesManager::LoadInfo(const std::string & resourcesFolder)
{
	using namespace std::experimental::filesystem;

	const recursive_directory_iterator end;
	for (recursive_directory_iterator i { resourcesFolder }; i != end; ++i)
	{
		if (i->path().extension().string() == ".ini")
			LoadSingleInfo(i->path().string());
	}
}

std::shared_ptr<Magma::Resource> Magma::ResourcesManager::Get(const std::string & name)
{
	auto f = m_resourcesInfo.find(name);
	if (f == m_resourcesInfo.end())
	{
		MAGMA_ERROR("Didn't find any resources with the name \"" + name + "\"");
		return nullptr;
	}
	else
	{
		if (f->second.GetResource() == nullptr)
		{
			// Resource not loaded, load it
			auto rsc = std::shared_ptr<Resource>(Resource::Create(f->second.GetType()), [](Resource* rsc) { rsc->Unload(); delete rsc; });
			if (rsc == nullptr)
			{
				MAGMA_ERROR("Failed to load resource, no resource type \"" + f->second.GetType() + "\" registered");
				return nullptr;
			}
			rsc->Load(f->second);
			f->second.SetResource(rsc);
			return rsc;
		}
		else return f->second.GetResource();
	}
}

void Magma::ResourcesManager::LoadSingleInfo(const std::string & path)
{
	std::ifstream ifs(path);
	if (!ifs.is_open())
	{
		MAGMA_WARNING("Failed to load resource info file \"" + path + "\"");
		return;
	}
	else
	{
		std::string infoFolder = "";
		{
			std::regex r(R"(.*\\)");
			std::smatch m;
			if (std::regex_search(path, m, r))
				infoFolder = path.substr(0, m.length());
		}

		ResourceInfo rinfo (infoFolder);
		ifs >> rinfo;
		auto insert = m_resourcesInfo.insert(std::make_pair(rinfo.GetName(), rinfo));
		if (insert.second == false)
		{
			std::string msg = "Failed to load resource info, there already exists a resource with the name \"" + rinfo.GetName() + "\"\n";
			msg += "Info file where conflict was found: \"" + path + "\"";
			MAGMA_WARNING(msg);
		}
#if MAGMA_VERBOSITY > 2
		else
		{
			std::cout << "Loaded resource info from \"" << path << "\"\n";
			std::cout << "Resource name: \"" << rinfo.GetName() << "\"\n";
			std::cout << "Resource type: \"" << rinfo.GetType() << "\"\n";
			std::cout << "Resource path: \"" << rinfo.GetPath() << "\"\n" << std::flush;
		}
#endif
	}
}

void Magma::ResourcesManager::DerivedInit()
{

}

void Magma::ResourcesManager::DerivedTerminate()
{

}
