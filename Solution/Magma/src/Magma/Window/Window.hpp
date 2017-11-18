#pragma once

#include "..\Utils\Math.hpp"
#include "..\Utils\Serializable.hpp"
#include "UIEvent.hpp"

namespace Magma
{
	/// <summary>
	///		Abstract interface class used to open and manage windows
	/// </summary>
	class Window : public Serializable
	{
	public:
		/// <summary>
		///		Enum containing window mode info
		/// </summary>
		enum class Mode
		{
			Invalid = -1,

			Windowed = 0,
			Fullscreen,
			Borderless,
			Splashscreen
		};

		/// <summary>
		///		Class containing window context settings
		/// </summary>
		class ContextSettings : public Serializable
		{
		public:
			ContextSettings();

			/// <summary>
			///		Default context settings
			/// </summary>
			static ContextSettings Default;

			uint32_t depthBits;
			uint32_t stencilBits;
			uint32_t antialiasingLevel;
			uint32_t majorVersion;
			uint32_t minorVersion;

		private:
			// Inherited via Serializable
			virtual void Serialize(std::ostream & stream) const override;
			virtual void Deserialize(std::istream & stream) override;
		};

		Window();
		virtual ~Window();

		/// <summary>
		///		Function used to check if a window is currently open
		/// </summary>
		/// <returns>True if window is open false if window is closed</returns>
		bool IsOpen();

		/// <summary>
		///		Opens the window with the specified dimensions and title
		/// </summary>
		/// <param name="windowSize">Window size</param>
		/// <param name="windowTitle">Window title</param>
		/// <param name="windowMode">Window mode</param>
		/// <param name="windowContextSettings">Advanced window context creation settings</param>
		void Open(glm::uvec2 windowSize, const std::string& windowTitle, Window::Mode windowMode = Window::Mode::Windowed, const Window::ContextSettings& windowContextSettings = Window::ContextSettings::Default);

		/// <summary>
		///		Opens the window using previous settings
		/// </summary>
		void Open();

		/// <summary>
		///		Closes the window if it is opened
		/// </summary>
		void Close();

		/// <summary>
		///		Sets a new window size
		/// </summary>
		/// <param name="windowSize">New window size</param>
		void Resize(glm::uvec2 windowSize);

		/// <summary>
		///		Sets a new window title
		/// </summary>
		/// <param name="windowTitle">New window title</param>
		void SetTitle(const std::string& windowTitle);

		/// <summary>
		///		Sets a new window mode
		/// </summary>
		/// <param name="windowMode">New window mode</param>
		void SetMode(Window::Mode windowMode);

		/// <summary>
		///		Returns window size
		/// </summary>
		/// <returns>Window size</returns>
		inline glm::uvec2 GetSize() { return m_windowSize; }

		/// <summary>
		///		Returns window aspect ratio (width / height)
		/// </summary>
		/// <returns>Window aspect ratio (width / height)</returns>
		inline float GetAspect() { return static_cast<float>(m_windowSize.x) / static_cast<float>(m_windowSize.y); }

		/// <summary>
		///		Returns window title
		/// </summary>
		/// <returns>Window title</returns>
		inline const std::string& GetTitle() { return m_windowTitle; }

		/// <summary>
		///		Returns window mode
		/// </summary>
		/// <returns>Window mode</returns>
		inline Window::Mode GetMode() { return m_windowMode; }

		/// <summary>
		///		Returns window context settings
		/// </summary>
		/// <returns>Window context settings</returns>
		inline const Window::ContextSettings& GetContextSettings() { return m_windowContextSettings; }

		/// <summary>
		///		Sets VSync enabled or not
		/// </summary>
		/// <param name="vsyncEnabled">True sets VSync on and false sets VSync off</param>
		virtual void SetVSyncEnabled(bool vsyncEnabled) = 0;

		/// <summary>
		///		Polls a event from the window
		/// </summary>
		/// <param name="event">Event that will be polled</param>
		/// <returns>True if there is an event was polled</returns>
		bool PollEvent(Magma::UIEvent& event);

		/// <summary>
		///		Displays changes in window to screen
		/// </summary>
		void Display();

		/// <summary>
		///		Makes this window the active one
		/// </summary>
		void MakeActive();

		/// <summary>
		///		Checks if keyboard key is pressed
		/// </summary>
		/// <param name="key">Keyboard key to check</param>
		/// <returns>True if keyboard key is pressed, otherwise false</returns>
		virtual bool IsKeyboardKeyPressed(Keyboard::Key key) = 0;

		/// <summary>
		///		Checks if mouse cursor is visible
		/// </summary>
		/// <returns>True if mouse cursor is visible, otherwise false</returns>
		virtual bool IsMouseCursorVisible() = 0;

		/// <summary>
		///		Checks if mouse button is pressed
		/// </summary>
		/// <param name="button">Mouse button to check</param>
		/// <returns>True if mouse button is pressed, otherwise false</returns>
		virtual bool IsMouseButtonPressed(Mouse::Button button) = 0;

		/// <summary>
		///		Sets the mouse cursor visibility
		/// </summary>
		/// <param name="visible">If true the mouse cursor is set to visible, otherwise to invisible</param>
		virtual void SetMouseCursorVisible(bool visible) = 0;

		/// <summary>
		///		Returns current mouse position in window coordinates
		/// </summary>
		/// <returns>Current mouse position in window coordinates</returns>
		virtual glm::vec2 GetMousePosition() = 0;

		/// <summary>
		///		Sets the mouse position in window coordinates
		/// </summary>
		/// <param name="mousePosition">New mouse position in window coordinates</param>
		virtual void SetMousePosition(glm::vec2 mousePosition) = 0;

		/// <summary>
		///		Returns the current active window
		/// </summary>
		/// <returns>Null if now window is active or the window currently active</returns>
		inline static Window* GetActive() { return s_activeWindow; }

	protected:
		static Window* s_activeWindow;

		glm::uvec2 m_windowSize;
		std::string m_windowTitle;
		Window::Mode m_windowMode;
		Window::ContextSettings m_windowContextSettings;

		virtual bool DerivedIsOpen() = 0;
		virtual void DerivedOpen() = 0;
		virtual void DerivedClose() = 0;
		virtual void DerivedResize() = 0;
		virtual void DerivedSetTitle() = 0;
		virtual void DerivedSetMode() = 0;
		virtual bool DerivedPollEvent(Magma::UIEvent& event) = 0;
		virtual void DerivedDisplay() = 0;
		virtual void DerivedMakeActive() = 0;

	private:
		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const override;
		virtual void Deserialize(std::istream & stream) override;
	};
}
