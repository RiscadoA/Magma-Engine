#pragma once

#include "RegistrableRegistry.hpp"

namespace Magma
{
	/// <summary>
	///		Adds registry functionality to a class type children
	/// </summary>
	template <typename BaseType>
	class Registrable
	{
	public:
		inline virtual ~Registrable() {}

		/// <summary>
		///		Creates a class object from it's name (must be a child of BaseType template argument)
		/// </summary>
		/// <param name="typeName">Type name of the class object</param>
		/// <returns>Created class object</returns>
		static BaseType* Create(const std::string& typeName);

		/// <summary>
		///		Destroys a class object that had been created with <code>Registrable::Create</code>
		/// </summary>
		/// <param name="registrable">Class object to destroy</param>
		static void Destroy(const BaseType* registrable);
	};

	template<typename BaseType>
	inline BaseType * Registrable<BaseType>::Create(const std::string & typeName)
	{
		::Magma::registrable::detail::RegistrableRegistry<BaseType>& reg = ::Magma::registrable::detail::GetRegistrableRegistry<BaseType>();
		::Magma::registrable::detail::RegistrableRegistry<BaseType>::iterator it = reg.find(typeName);

		if (it == reg.end())
			return nullptr; // No registrable was registred with this name

		::Magma::registrable::detail::CreateRegistrableFunc<BaseType> func = it->second;
		return func();
	}

	template<typename BaseType>
	inline void Registrable<BaseType>::Destroy(const BaseType * registrable)
	{
		delete registrable;
	}

	/// <summary>
	///		Registers a child type of BASE_TYPE with the chosen name, so it can later be created with <code>Registrable::Create</code>
	/// </summary>
	/// <param name="BASE_TYPE">Base class type</param>
	/// <param name="TYPE">Child class type</param>
	/// <param name="NAME">Child class type name</param>
#define MAGMA_REGISTER(BASE_TYPE, TYPE, NAME) \
	namespace registrable {\
	namespace detail { \
	namespace { \
		template <class BaseType, class T> \
		class RegistrableRegistration; \
		\
		template <> \
		class RegistrableRegistration<BASE_TYPE, TYPE> { \
			static const ::Magma::registrable::detail::RegistryEntry<BASE_TYPE, TYPE>& reg; \
		}; \
		\
		const ::Magma::registrable::detail::RegistryEntry<BASE_TYPE, TYPE>& \
			RegistrableRegistration<BASE_TYPE, TYPE>::reg = \
				::Magma::registrable::detail::RegistryEntry<BASE_TYPE, TYPE>::Instance(NAME); \
	}}}
}
