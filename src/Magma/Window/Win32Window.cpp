#include "Win32Window.hpp"

#include <Windowsx.h>

Magma::Keyboard::Key WindowsToMagmaKey(WPARAM key)
{
	using namespace Magma;

	switch (key)
	{
		case VK_BACK: return Keyboard::Key::BackSpace; break;
		case VK_TAB: return Keyboard::Key::Tab; break;
		case VK_RETURN: return Keyboard::Key::Return; break;
		case VK_LSHIFT: return Keyboard::Key::LShift; break;
		case VK_RSHIFT: return Keyboard::Key::RShift; break;
		case VK_LCONTROL: return Keyboard::Key::LControl; break;
		case VK_RCONTROL: return Keyboard::Key::RControl; break;
		case VK_LMENU: return Keyboard::Key::LAlt; break;
		case VK_RMENU: return Keyboard::Key::RAlt; break;
		case VK_PAUSE: return Keyboard::Key::Pause; break;
		case VK_ESCAPE: return Keyboard::Key::Escape; break;
		case VK_SPACE: return Keyboard::Key::Space; break;
		case VK_PRIOR: return Keyboard::Key::PageUp; break;
		case VK_NEXT: return Keyboard::Key::PageDown; break;
		case VK_END: return Keyboard::Key::End; break;
		case VK_HOME: return Keyboard::Key::Home; break;
		case VK_LEFT: return Keyboard::Key::Left; break;
		case VK_UP: return Keyboard::Key::Up; break;
		case VK_DOWN: return Keyboard::Key::Down; break;
		case VK_RIGHT: return Keyboard::Key::Right; break;
		case VK_INSERT: return Keyboard::Key::Insert; break;
		case VK_DELETE: return Keyboard::Key::Delete; break;
		case 0x30: return Keyboard::Key::Num0; break;
		case 0x31: return Keyboard::Key::Num1; break;
		case 0x32: return Keyboard::Key::Num2; break;
		case 0x33: return Keyboard::Key::Num3; break;
		case 0x34: return Keyboard::Key::Num4; break;
		case 0x35: return Keyboard::Key::Num5; break;
		case 0x36: return Keyboard::Key::Num6; break;
		case 0x37: return Keyboard::Key::Num7; break;
		case 0x38: return Keyboard::Key::Num8; break;
		case 0x39: return Keyboard::Key::Num9; break;
		case 0x41: return Keyboard::Key::A; break;
		case 0x42: return Keyboard::Key::B; break;
		case 0x43: return Keyboard::Key::C; break;
		case 0x44: return Keyboard::Key::D; break;
		case 0x45: return Keyboard::Key::E; break;
		case 0x46: return Keyboard::Key::F; break;
		case 0x47: return Keyboard::Key::G; break;
		case 0x48: return Keyboard::Key::H; break;
		case 0x49: return Keyboard::Key::I; break;
		case 0x4A: return Keyboard::Key::J; break;
		case 0x4B: return Keyboard::Key::K; break;
		case 0x4C: return Keyboard::Key::L; break;
		case 0x4D: return Keyboard::Key::M; break;
		case 0x4E: return Keyboard::Key::N; break;
		case 0x4F: return Keyboard::Key::O; break;
		case 0x50: return Keyboard::Key::P; break;
		case 0x51: return Keyboard::Key::Q; break;
		case 0x52: return Keyboard::Key::R; break;
		case 0x53: return Keyboard::Key::S; break;
		case 0x54: return Keyboard::Key::T; break;
		case 0x55: return Keyboard::Key::U; break;
		case 0x56: return Keyboard::Key::V; break;
		case 0x57: return Keyboard::Key::W; break;
		case 0x58: return Keyboard::Key::X; break;
		case 0x59: return Keyboard::Key::Y; break;
		case 0x5A: return Keyboard::Key::Z; break;
		case VK_NUMPAD0: return Keyboard::Key::Numpad0; break;
		case VK_NUMPAD1: return Keyboard::Key::Numpad1; break;
		case VK_NUMPAD2: return Keyboard::Key::Numpad2; break;
		case VK_NUMPAD3: return Keyboard::Key::Numpad3; break;
		case VK_NUMPAD4: return Keyboard::Key::Numpad4; break;
		case VK_NUMPAD5: return Keyboard::Key::Numpad5; break;
		case VK_NUMPAD6: return Keyboard::Key::Numpad6; break;
		case VK_NUMPAD7: return Keyboard::Key::Numpad7; break;
		case VK_NUMPAD8: return Keyboard::Key::Numpad8; break;
		case VK_NUMPAD9: return Keyboard::Key::Numpad9; break;
		case VK_MULTIPLY: return Keyboard::Key::Multiply; break;
		case VK_ADD: return Keyboard::Key::Add; break;
		case VK_SUBTRACT: return Keyboard::Key::Subtract; break;
		case VK_DECIMAL: return Keyboard::Key::Period; break;
		case VK_SEPARATOR: return Keyboard::Key::Return;
		case VK_F1: return Keyboard::Key::F1;
		case VK_F2: return Keyboard::Key::F2;
		case VK_F3: return Keyboard::Key::F3;
		case VK_F4: return Keyboard::Key::F4;
		case VK_F5: return Keyboard::Key::F5;
		case VK_F6: return Keyboard::Key::F6;
		case VK_F7: return Keyboard::Key::F7;
		case VK_F8: return Keyboard::Key::F8;
		case VK_F9: return Keyboard::Key::F9;
		case VK_F10: return Keyboard::Key::F10;
		case VK_F11: return Keyboard::Key::F11;
		case VK_F12: return Keyboard::Key::F12;
		case VK_F13: return Keyboard::Key::F13;
		case VK_F14: return Keyboard::Key::F14;
		case VK_F15: return Keyboard::Key::F15;
	}
	return Keyboard::Key::Invalid;
}

Magma::Mouse::Button WindowsToMagmaMB(WPARAM mb)
{
	using namespace Magma;

	switch (mb)
	{
		case VK_LBUTTON: return Mouse::Button::Left; break;
		case VK_RBUTTON: return Mouse::Button::Right; break;
		case VK_MBUTTON: return Mouse::Button::Middle; break;
		case VK_XBUTTON1: return Mouse::Button::Extra1; break;
		case VK_XBUTTON2: return Mouse::Button::Extra2; break;
	}
	return Mouse::Button::Invalid;
}

LRESULT CALLBACK Magma::Win32Window::WindowProc(HWND hWnd,
							UINT message,
							WPARAM wParam,
							LPARAM lParam)
{
	static bool mouseEntered = true;

	auto win = dynamic_cast<Win32Window*>(Window::GetActive());
	if (win != nullptr)
	{
		switch (message)
		{
			case WM_DESTROY:
			{
				// Close the window
				win->Close();
				return 0;
			} break;

			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				UIEvent event;
				if (message == WM_KEYDOWN)
					event.type = UIEvent::Type::KeyPressed;
				else if (message == WM_KEYUP)
					event.type = UIEvent::Type::KeyReleased;
				event.key.code = WindowsToMagmaKey(wParam);
				event.key.shift = GetKeyState(VK_SHIFT);
				event.key.control = GetKeyState(VK_CONTROL);
				event.key.alt = GetKeyState(VK_MENU);
				win->m_eventQueue.push(event);
				if (event.key.code != Magma::Keyboard::Key::Invalid)
					if (event.type == UIEvent::Type::KeyPressed)
						win->m_keyboardKeyStates[static_cast<size_t>(event.key.code)] = true;
					else
						win->m_keyboardKeyStates[static_cast<size_t>(event.key.code)] = false;
				return 0;
			} break;

			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			{
				UIEvent event;
				if (message == WM_LBUTTONDOWN)
					event.type = UIEvent::Type::MouseButtonPressed;
				else
					event.type = UIEvent::Type::MouseButtonReleased;
				event.mouseButton.button = Mouse::Button::Left;
				event.mouseButton.x = GET_X_LPARAM(lParam);
				event.mouseButton.y = GET_Y_LPARAM(lParam);
				
				
				win->m_eventQueue.push(event);
				if (event.mouseButton.button != Magma::Mouse::Button::Invalid)
					if (event.type == UIEvent::Type::MouseButtonPressed)
						win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = true;
					else
						win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = false;
				return 0;
			} break;

			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			{
				UIEvent event;
				if (message == WM_RBUTTONDOWN)
					event.type = UIEvent::Type::MouseButtonPressed;
				else
					event.type = UIEvent::Type::MouseButtonReleased;
				event.mouseButton.button = Mouse::Button::Right;
				event.mouseButton.x = GET_X_LPARAM(lParam);
				event.mouseButton.y = GET_Y_LPARAM(lParam);
				win->m_eventQueue.push(event);
				if (event.mouseButton.button != Magma::Mouse::Button::Invalid)
					if (event.type == UIEvent::Type::MouseButtonPressed)
						win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = true;
					else
						win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = false;
				return 0;
			} break;

			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			{
				UIEvent event;
				if (message == WM_MBUTTONDOWN)
					event.type = UIEvent::Type::MouseButtonPressed;
				else
					event.type = UIEvent::Type::MouseButtonReleased;
				event.mouseButton.button = Mouse::Button::Middle;
				event.mouseButton.x = GET_X_LPARAM(lParam);
				event.mouseButton.y = GET_Y_LPARAM(lParam);
				win->m_eventQueue.push(event);
				if (event.mouseButton.button != Magma::Mouse::Button::Invalid)
					if (event.type == UIEvent::Type::MouseButtonPressed)
						win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = true;
					else
						win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = false;
				return 0;
			} break;

			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			{
				UIEvent event;
				if (message == WM_XBUTTONDOWN)
					event.type = UIEvent::Type::MouseButtonPressed;
				else
					event.type = UIEvent::Type::MouseButtonReleased;
				event.mouseButton.button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? Mouse::Button::Extra1 : Mouse::Button::Extra2;
				event.mouseButton.x = GET_X_LPARAM(lParam);
				event.mouseButton.y = GET_Y_LPARAM(lParam);
				win->m_eventQueue.push(event);
				if (event.mouseButton.button != Magma::Mouse::Button::Invalid)
					if (event.type == UIEvent::Type::MouseButtonPressed)
						win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = true;
					else
						win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = false;
				return 0;
			} break;

			case WM_MOUSEWHEEL:
			{
				UIEvent event;
				event.type = UIEvent::Type::MouseWheelScrolled;
				event.mouseWheelScrolled.delta = GET_WHEEL_DELTA_WPARAM(wParam);
				event.mouseWheelScrolled.x = GET_X_LPARAM(lParam);
				event.mouseWheelScrolled.y = GET_Y_LPARAM(lParam);
				event.mouseWheelScrolled.wheel = Mouse::Wheel::Vertical;
				win->m_eventQueue.push(event);
			} break;

			case WM_MOUSEMOVE:
			{
				if (!mouseEntered)
				{
					mouseEntered = true;
					UIEvent event;
					event.type = UIEvent::Type::MouseEntered;
					win->m_eventQueue.push(event);
				}

				UIEvent event;
				event.type = UIEvent::Type::MouseMoved;
				event.mouseMoved.x = GET_X_LPARAM(lParam);
				event.mouseMoved.y = GET_Y_LPARAM(lParam);
				win->m_eventQueue.push(event);
				return 0;
			} break;

			case WM_MOUSELEAVE:
			{
				mouseEntered = false;
				UIEvent event;
				event.type = UIEvent::Type::MouseLeft;
				win->m_eventQueue.push(event);
				return 0;
			} break;
		}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

Magma::Win32Window::Win32Window(HINSTANCE hInstance, int nCmdShow)
	: m_hInstance(hInstance), m_nCmdShow(nCmdShow)
{
	
}

Magma::Win32Window::~Win32Window()
{

}

void Magma::Win32Window::SetVSyncEnabled(bool vsyncEnabled)
{

}

bool Magma::Win32Window::IsKeyboardKeyPressed(Keyboard::Key key)
{
	return m_keyboardKeyStates[static_cast<size_t>(key)];
}

bool Magma::Win32Window::IsMouseCursorVisible()
{
	return m_cursorVisible;
}

bool Magma::Win32Window::IsMouseButtonPressed(Mouse::Button button)
{
	return m_mouseButtonStates[static_cast<size_t>(button)];
}

void Magma::Win32Window::SetMouseCursorVisible(bool visible)
{
	m_cursorVisible = visible;
	ShowCursor(visible);
}

glm::vec2 Magma::Win32Window::GetMousePosition()
{
	return glm::vec2();
}

void Magma::Win32Window::SetMousePosition(glm::vec2 mousePosition)
{

}

bool Magma::Win32Window::DerivedIsOpen()
{
	return IsWindow(m_hWnd);
}

void Magma::Win32Window::DerivedOpen()
{
	if (IsWindow(m_hWnd))
		DestroyWindow(m_hWnd);

	ZeroMemory(&m_wc, sizeof(WNDCLASSEX));

	m_wc.cbSize = sizeof(WNDCLASSEX);
	m_wc.style = CS_HREDRAW | CS_VREDRAW;
	m_wc.lpfnWndProc = WindowProc;
	m_wc.hInstance = m_hInstance;
	m_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	m_wc.lpszClassName = "Magma_Win32Window_Class";

	RegisterClassEx(&m_wc);

	m_hWnd = CreateWindowEx(NULL,
							"Magma_Win32Window_Class",
							m_windowTitle.c_str(),
							WS_OVERLAPPEDWINDOW,
							0,
							0,
							m_windowSize.x,
							m_windowSize.y,
							NULL,
							NULL,
							m_hInstance,
							NULL);

	ShowWindow(m_hWnd, m_nCmdShow);
}

void Magma::Win32Window::DerivedClose()
{
	DestroyWindow(m_hWnd);
}

void Magma::Win32Window::DerivedResize()
{
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, m_windowSize.x, m_windowSize.y, SWP_SHOWWINDOW);
}

void Magma::Win32Window::DerivedSetTitle()
{
	SetWindowText(m_hWnd, m_windowTitle.c_str());
}

void Magma::Win32Window::DerivedSetMode()
{

}

bool Magma::Win32Window::DerivedPollEvent(Magma::UIEvent & event)
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	if (m_eventQueue.empty())
		return false;
	else
	{
		event = m_eventQueue.front();
		m_eventQueue.pop();
		return true;
	}
}

void Magma::Win32Window::DerivedDisplay()
{

}

void Magma::Win32Window::DerivedMakeActive()
{

}
