#pragma once

#include "Window.hpp"

#include <Windows.h>
#include <queue>

namespace Magma
{
	class Win32Window : public Window
	{
	public:
		Win32Window(HINSTANCE hInstance, int nCmdShow);
		virtual ~Win32Window() final;

	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd,
									UINT message,
									WPARAM wParam,
									LPARAM lParam);

		// Inherited via Window
		virtual void SetVSyncEnabled(bool vsyncEnabled) override;
		virtual bool IsKeyboardKeyPressed(Keyboard::Key key) override;
		virtual bool IsMouseCursorVisible() override;
		virtual bool IsMouseButtonPressed(Mouse::Button button) override;
		virtual void SetMouseCursorVisible(bool visible) override;
		virtual glm::vec2 GetMousePosition() override;
		virtual void SetMousePosition(glm::vec2 mousePosition) override;
		virtual bool DerivedIsOpen() override;
		virtual void DerivedOpen() override;
		virtual void DerivedClose() override;
		virtual void DerivedResize() override;
		virtual void DerivedSetTitle() override;
		virtual void DerivedSetMode() override;
		virtual bool DerivedPollEvent(Magma::UIEvent & event) override;
		virtual void DerivedDisplay() override;
		virtual void DerivedMakeActive() override;

		HINSTANCE m_hInstance;
		HWND m_hWnd;
		WNDCLASSEX m_wc;
		int m_nCmdShow;

		bool m_cursorVisible = true;

		std::queue<Magma::UIEvent> m_eventQueue;

		bool m_mouseButtonStates[static_cast<size_t>(Mouse::Button::Count)];
		bool m_keyboardKeyStates[static_cast<size_t>(Keyboard::Key::Count)];
	};
}