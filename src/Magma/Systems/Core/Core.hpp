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

	class Terminal;
	class ResourcesManager;
	class Input;
	class Window;
	class RenderDevice;

	/// <summary>
	///		Engine Locator. Stores pointers to engine systems.
	/// </summary>
	struct Locator
	{
		Locator() = default;
		~Locator() = default;
		// Delete copy constructor
		Locator(const Locator&) = delete;
		Locator& operator=(const Locator&) = delete;

		/// <summary>
		///		Engine message bus
		/// </summary>
		std::shared_ptr<MessageBus> msgBus;

		/// <summary>
		///		Engine core
		/// </summary>
		std::shared_ptr<Core> core;

		/// <summary>
		///		Engine terminal (executes commands from the console)
		/// </summary>
		std::shared_ptr<Terminal> terminal;

		/// <summary>
		///		Engine resources manager
		/// </summary>
		std::shared_ptr<ResourcesManager> resourcesManager;

		/// <summary>
		///		Engine input system
		/// </summary>
		std::shared_ptr<Input> input;
		
		/// <summary>
		///		Engine window
		/// </summary>
		std::shared_ptr<Window> window;

		/// <summary>
		///		Engine render device
		/// </summary>
		std::shared_ptr<RenderDevice> renderDevice;
	};
}

/// <summary>
///		Magma custom init function.
///		Called on engine startup.
/// </summary>
extern int MagmaMain(const Magma::Locator&);
