#pragma once

#include "..\MessageBus.hpp"

#include <mutex>

namespace Magma
{
	/// <summary>
	///		Terminal system.
	///		Reads commands from the console and executes them.
	/// </summary>
	class Terminal : public MessageListener
	{
	public:
		void Update();

		/// <summary>
		///		Adds a command to the terminal
		/// </summary>
		/// <param name="name">Command name</param>
		/// <param name="command">Command function</param>
		static void AddCommand(const std::string& name, std::function<void(const std::vector<std::string>&)> command);

		/// <summary>
		///		Removes a command from the terminal
		/// </summary>
		/// <param name="name">Command name</param>
		static void RemoveCommand(const std::string& name);

		/// <summary>
		///		Executes a command
		/// </summary>
		/// <param name="name">Command name</param>
		/// <param name="parameters">Command parameters</param>
		static void ExecuteCommand(const std::string& name, const std::vector<std::string>& parameters);

	private:
		static std::map<std::string, std::function<void(const std::vector<std::string>&)>> s_commands;
		static std::mutex s_commands_mutex;

		std::uint64_t m_iCallback;

		void OnInput(const std::string& in);

		// Inherited via MessageListener
		virtual void DerivedInit() override;
		virtual void DerivedTerminate() override;
	};
}