#pragma once

#include "Window.hpp"

#include <atomic>
#include <queue>

struct GLFWwindow;

namespace Magma
{
	/// <summary>
	///		Implements Window abstract interface class using GLFW
	/// </summary>
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow();
		virtual ~GLFWWindow() final;

		/// <summary>
		///		Gets this window GLFW handle
		/// </summary>
		/// <returns>This window GLFW handle</returns>
		inline GLFWwindow* GetGLFWWindow() { return m_glfwWindow; }

		/// <summary>
		///		Converts a GLFW mouse button to an Magma mouse button
		/// </summary>
		/// <param name="button">GLFW Mouse button to be converted</param>
		/// <returns>Corresponding Magma mouse button</returns>
		static Mouse::Button GLFWMButtonToMagma(int button);

		/// <summary>
		///		Converts a GLFW keyboard key to an Magma keyboard key
		/// </summary>
		/// <param name="key">GLFW keyboard ey to be converted</param>
		/// <returns>Corresponding Magma keyboard key</returns>
		static Keyboard::Key GLFWKeyToMagma(int key);

	protected:
		// Inherited via Window
		virtual bool DerivedIsOpen() final;
		virtual void DerivedOpen() final;
		virtual void DerivedClose() final;
		virtual void DerivedResize() final;
		virtual void DerivedSetTitle() final;
		virtual void DerivedSetMode() final;
		virtual void SetVSyncEnabled(bool vsyncEnabled) final;
		virtual bool DerivedPollEvent(Magma::UIEvent& event) final;
		virtual void DerivedDisplay() final;
		virtual glm::vec2 GetMousePosition() final;
		virtual void SetMousePosition(glm::vec2 mousePosition) final;
		virtual bool IsKeyboardKeyPressed(Keyboard::Key key) override;
		virtual bool IsMouseCursorVisible() override;
		virtual bool IsMouseButtonPressed(Mouse::Button button) override;
		virtual void SetMouseCursorVisible(bool visible) override;
		virtual void DerivedMakeActive() override;

	private:
		static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void GLFWMouseScrollCallback(GLFWwindow* window, double x, double y);
		static void GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void GLFWCursorEnterCallback(GLFWwindow* window, int entered);
		static void GLFWCharCallback(GLFWwindow* window, unsigned int codePoint);
		static void GLFWResizeCallback(GLFWwindow* window, int width, int height);
		static void GLFWFocusCallback(GLFWwindow* window, int focused);
		static void GLFWCloseCallback(GLFWwindow* window);

		static std::atomic<int> s_windowNumber;

		GLFWwindow* m_glfwWindow;

		bool m_mouseButtonStates[static_cast<size_t>(Mouse::Button::Count)];
		bool m_keyboardKeyStates[static_cast<size_t>(Keyboard::Key::Count)];

		bool m_mouseCursorVisible = true;

		std::queue<Magma::UIEvent> m_eventQueue;
	};
}
