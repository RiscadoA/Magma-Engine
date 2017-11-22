#pragma once

#include "..\MessageBus.hpp"

namespace Magma
{
	/// <summary>
	///		Engine core system.
	///		Handles primary stuff such as engine exit.
	/// </summary>
	class Core : public MessageListener
	{
	public:
		void Update();
		
		inline bool IsRunning() { return m_running; }

	private:
		// Inherited via MessageListener
		virtual void DerivedInit() override;
		virtual void DerivedTerminate() override;

		std::atomic<bool> m_running;
	};
}
