#pragma once

#include "..\MessageBus.hpp"

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
		void OnInput(const std::string& in);

	private:
		std::uint64_t m_iCallback;

		// Inherited via MessageListener
		virtual void DerivedInit() override;
		virtual void DerivedTerminate() override;
	};
}