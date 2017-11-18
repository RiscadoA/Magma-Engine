#pragma once

#include <sstream>
#include <map>
#include <mutex>

#include "Serializable.hpp"

namespace Magma
{
	/// <summary>
	///		Variant type object used to store global variables (settings, etc)
	/// </summary>
	class GlobalVar : public Serializable
	{
	public:
		template <typename T>
		operator T()
		{
			m_mutex.lock(); com
			std::stringstream ss;
			ss << m_value;
			T val;
			ss >> val;
			m_mutex.unlock();
			return val;
		}

		template <typename T>
		GlobalVar& operator=(const T& rhs)
		{
			m_mutex.lock();
			std::stringstream ss;
			ss << rhs;
			m_value = ss.str();
			m_mutex.unlock();
			return *this;
		}

	private:
		std::string m_value;
		std::mutex m_mutex;

		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const override;
		virtual void Deserialize(std::istream & stream) override;
	};

	static class Globals
	{
	public:
		Globals() = delete;
		~Globals() = delete;

		static GlobalVar& Get(const std::string& varName);

	private:
		static std::map<std::string, GlobalVar> s_globals;
		static std::mutex s_mutex;
	};
}