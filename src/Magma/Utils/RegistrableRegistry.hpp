#pragma once

#include <map>
#include <string>
#include <utility>
#include <iostream>

#include "..\Utils\Utils.hpp"

namespace Magma
{
	template <typename T>
	class Registrable;

	namespace registrable
	{
		namespace detail
		{
			template <class BaseType>
			using CreateRegistrableFunc = BaseType*(*)();
			template <class BaseType>
			using RegistrableRegistry = std::map<std::string, CreateRegistrableFunc<BaseType>>;

			template <typename BaseType>
			inline RegistrableRegistry<BaseType>& GetRegistrableRegistry()
			{
				static RegistrableRegistry<BaseType> reg;
				return reg;
			}

			template <class BaseType, class T>
			BaseType* CreateRegistrable() { return new T; }

			template <class BaseType, class T>
			struct RegistryEntry
			{
			public:
				static RegistryEntry<BaseType, T>& Instance(const std::string& typeName)
				{
					static RegistryEntry<BaseType, T> inst(typeName);
					return inst;
				}

			private:
				RegistryEntry(const std::string& typeName)
				{
					RegistrableRegistry<BaseType>& reg = GetRegistrableRegistry<BaseType>();
					CreateRegistrableFunc<BaseType> func = CreateRegistrable<BaseType, T>;

					std::pair<RegistrableRegistry<BaseType>::iterator, bool> ret = reg.insert(RegistrableRegistry<BaseType>::value_type(typeName, func));

					if (ret.second == false)
					{
						// Registrable already registered with this name
						MAGMA_WARNING("Failed to register entry, there is already another entry with the same name (\"" + typeName + "\")");
					}
				}

				RegistryEntry(const RegistryEntry<BaseType, T>&) = delete;
				RegistryEntry& operator=(const RegistryEntry<BaseType, T>&) = delete;
			};
		}
	}
}
