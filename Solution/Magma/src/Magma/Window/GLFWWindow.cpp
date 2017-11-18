#include "GLFWWindow.hpp"

#include <GLFW\glfw3.h>

#include "..\Utils\Utils.hpp"

std::atomic<int> Magma::GLFWWindow::s_windowNumber = 0;

void GLFWErrorCallback(int error, const char* description)
{
	std::string err = "GLFW Error '" + std::to_string(error) + "':" + description;
	MAGMA_ERROR(err);
}

void Magma::GLFWWindow::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto win = dynamic_cast<GLFWWindow*>(Window::GetActive());
	if (win == nullptr)
		return;
	else
	{
		if (win->m_glfwWindow != window)
			MAGMA_WARNING("GLFW callback window doesn't correspond to active window");
		UIEvent event;
		if (action == GLFW_PRESS)
			event.type = UIEvent::Type::KeyPressed;
		else if (action == GLFW_RELEASE)
			event.type = UIEvent::Type::KeyReleased;
		else
			return;
		event.key.code = GLFWWindow::GLFWKeyToMagma(key);
		event.key.shift = (mods & GLFW_MOD_SHIFT) != 0;
		event.key.control = (mods & GLFW_MOD_CONTROL) != 0;
		event.key.alt = (mods & GLFW_MOD_ALT) != 0;
		win->m_eventQueue.push(event);
		if (event.key.code != Magma::Keyboard::Key::Invalid)
			if (action == GLFW_PRESS)
				win->m_keyboardKeyStates[static_cast<size_t>(event.key.code)] = true;
			else if (action == GLFW_RELEASE)
				win->m_keyboardKeyStates[static_cast<size_t>(event.key.code)] = false;
	}
}

void Magma::GLFWWindow::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto win = dynamic_cast<GLFWWindow*>(Window::GetActive());
	if (win == nullptr)
		return;
	else
	{
		if (win->m_glfwWindow != window)
			MAGMA_WARNING("GLFW callback window doesn't correspond to active window");
		UIEvent event;
		if (action == GLFW_PRESS)
			event.type = UIEvent::Type::MouseButtonPressed;
		else if (action == GLFW_RELEASE)
			event.type = UIEvent::Type::MouseButtonReleased;
		else
			return;
		event.mouseButton.button = GLFWWindow::GLFWMButtonToMagma(button);
		auto mPos = win->GetMousePosition();
		event.mouseButton.x = mPos.x;
		event.mouseButton.y = mPos.y;
		win->m_eventQueue.push(event);
		if (event.mouseButton.button != Magma::Mouse::Button::Invalid)
			if (action == GLFW_PRESS)
				win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = true;
			else if (action == GLFW_RELEASE)
				win->m_mouseButtonStates[static_cast<size_t>(event.mouseButtonPressed.button)] = false;
	}
}

void Magma::GLFWWindow::GLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
{
	auto win = dynamic_cast<GLFWWindow*>(Window::GetActive());
	if (win == nullptr)
		return;
	else
	{
		if (win->m_glfwWindow != window)
			MAGMA_WARNING("GLFW callback window doesn't correspond to active window");
		if (y != 0.0)
		{
			UIEvent event;
			event.type = Magma::UIEvent::Type::MouseWheelScrolled;
			event.mouseWheelScrolled.wheel = Magma::Mouse::Wheel::Vertical;
			event.mouseWheelScrolled.delta = y;
			auto mPos = win->GetMousePosition();
			event.mouseWheelScrolled.x = mPos.x;
			event.mouseWheelScrolled.y = mPos.y;
			win->m_eventQueue.push(event);
		}
		if (x != 0.0)
		{
			UIEvent event;
			event.type = Magma::UIEvent::Type::MouseWheelScrolled;
			event.mouseWheelScrolled.wheel = Magma::Mouse::Wheel::Horizontal;
			event.mouseWheelScrolled.delta = x;
			auto mPos = win->GetMousePosition();
			event.mouseWheelScrolled.x = mPos.x;
			event.mouseWheelScrolled.y = mPos.y;
			win->m_eventQueue.push(event);
		}
	}
	
}

void Magma::GLFWWindow::GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	auto win = dynamic_cast<GLFWWindow*>(Window::GetActive());
	if (win == nullptr)
		return;
	else
	{
		if (win->m_glfwWindow != window)
			MAGMA_WARNING("GLFW callback window doesn't correspond to active window");
		UIEvent event;
		event.type = Magma::UIEvent::Type::MouseMoved;
		event.mouseMoved.x = xpos;
		event.mouseMoved.y = ypos;
		win->m_eventQueue.push(event);
	}
}

void Magma::GLFWWindow::GLFWCursorEnterCallback(GLFWwindow * window, int entered)
{
	auto win = dynamic_cast<GLFWWindow*>(Window::GetActive());
	if (win == nullptr)
		return;
	else
	{
		if (win->m_glfwWindow != window)
			MAGMA_WARNING("GLFW callback window doesn't correspond to active window");
		UIEvent event;
		event.type = (entered == 1) ? Magma::UIEvent::Type::MouseEntered : Magma::UIEvent::Type::MouseLeft;
		win->m_eventQueue.push(event);
	}
}

void Magma::GLFWWindow::GLFWCharCallback(GLFWwindow * window, unsigned int codePoint)
{
	auto win = dynamic_cast<GLFWWindow*>(Window::GetActive());
	if (win == nullptr)
		return;
	else
	{
		if (win->m_glfwWindow != window)
			MAGMA_WARNING("GLFW callback window doesn't correspond to active window");
		UIEvent event;
		event.type = Magma::UIEvent::Type::TextEntered;
		event.textEntered.unicode = codePoint;
		win->m_eventQueue.push(event);
	}
}

void Magma::GLFWWindow::GLFWResizeCallback(GLFWwindow * window, int width, int height)
{
	auto win = dynamic_cast<GLFWWindow*>(Window::GetActive());
	if (win == nullptr)
		return;
	else
	{
		if (win->m_glfwWindow != window)
			MAGMA_WARNING("GLFW callback window doesn't correspond to active window");
		UIEvent event;
		event.type = Magma::UIEvent::Type::Resized;
		event.resized.width = width;
		event.resized.height = height;
		win->m_windowSize.x = width;
		win->m_windowSize.y = height;
		win->m_eventQueue.push(event);
	}
}

void Magma::GLFWWindow::GLFWFocusCallback(GLFWwindow * window, int focused)
{
	auto win = dynamic_cast<GLFWWindow*>(Window::GetActive());
	if (win == nullptr)
		return;
	else
	{
		if (win->m_glfwWindow != window)
			MAGMA_WARNING("GLFW callback window doesn't correspond to active window");
		UIEvent event;
		event.type = (focused == 1) ? Magma::UIEvent::Type::GainedFocus : Magma::UIEvent::Type::LostFocus;
		win->m_eventQueue.push(event);
	}
}

void Magma::GLFWWindow::GLFWCloseCallback(GLFWwindow * window)
{
	auto win = dynamic_cast<GLFWWindow*>(Window::GetActive());
	if (win == nullptr)
		return;
	else
	{
		if (win->m_glfwWindow != window)
			MAGMA_WARNING("GLFW callback window doesn't correspond to active window");
		UIEvent event;
		event.type = UIEvent::Type::Closed;
		win->m_eventQueue.push(event);
	}
}

void Magma::GLFWWindow::DerivedMakeActive()
{
	glfwMakeContextCurrent(m_glfwWindow);
}

Magma::GLFWWindow::GLFWWindow()
{
	m_glfwWindow = nullptr;

	if (s_windowNumber == 0)
	{
		glfwSetErrorCallback(GLFWErrorCallback);
		if (!glfwInit())
		{
			MAGMA_ERROR("Failed to construct GLFWWindow because GLFW failed to init");
		}
	}
	++s_windowNumber;
}

Magma::GLFWWindow::~GLFWWindow()
{
	if (m_glfwWindow != nullptr)
	{
		glfwDestroyWindow(m_glfwWindow);
		m_glfwWindow = nullptr;
	}

	--s_windowNumber;
	if (s_windowNumber == 0)
		glfwTerminate();
}

Magma::Mouse::Button Magma::GLFWWindow::GLFWMButtonToMagma(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT: return Magma::Mouse::Button::Left; break;
	case GLFW_MOUSE_BUTTON_RIGHT: return Magma::Mouse::Button::Right; break;
	case GLFW_MOUSE_BUTTON_MIDDLE: return Magma::Mouse::Button::Middle; break;
	case GLFW_MOUSE_BUTTON_4: return Magma::Mouse::Button::Extra1; break;
	case GLFW_MOUSE_BUTTON_5: return Magma::Mouse::Button::Extra2; break;
	}
	return Mouse::Button::Invalid;
}

Magma::Keyboard::Key Magma::GLFWWindow::GLFWKeyToMagma(int key)
{
	switch (key)
	{
#define ADDKEY(glfw, key) case GLFW_KEY_##glfw: return Magma::Keyboard::Key::key; break
		ADDKEY(A, A);
		ADDKEY(B, B);
		ADDKEY(C, C);
		ADDKEY(D, D);
		ADDKEY(E, E);
		ADDKEY(F, F);
		ADDKEY(G, G);
		ADDKEY(H, H);
		ADDKEY(I, I);
		ADDKEY(J, J);
		ADDKEY(K, K);
		ADDKEY(L, L);
		ADDKEY(M, M);
		ADDKEY(N, N);
		ADDKEY(O, O);
		ADDKEY(P, P);
		ADDKEY(Q, Q);
		ADDKEY(R, R);
		ADDKEY(S, S);
		ADDKEY(T, T);
		ADDKEY(U, U);
		ADDKEY(V, V);
		ADDKEY(W, W);
		ADDKEY(X, X);
		ADDKEY(Y, Y);
		ADDKEY(Z, Z);
		ADDKEY(0, Num0);
		ADDKEY(1, Num1);
		ADDKEY(2, Num2);
		ADDKEY(3, Num3);
		ADDKEY(4, Num4);
		ADDKEY(5, Num5);
		ADDKEY(6, Num6);
		ADDKEY(7, Num7);
		ADDKEY(8, Num8);
		ADDKEY(9, Num9);
		ADDKEY(ESCAPE, Escape);
		ADDKEY(LEFT_CONTROL, LControl);
		ADDKEY(LEFT_SHIFT, LShift);
		ADDKEY(LEFT_ALT, LAlt);
		ADDKEY(LEFT_SUPER, LSystem);
		ADDKEY(RIGHT_CONTROL, RControl);
		ADDKEY(RIGHT_SHIFT, RShift);
		ADDKEY(RIGHT_ALT, RAlt);
		ADDKEY(RIGHT_SUPER, RSystem);
		ADDKEY(MENU, Menu);
		ADDKEY(LEFT_BRACKET, LBracket);
		ADDKEY(RIGHT_BRACKET, RBracket);
		ADDKEY(SEMICOLON, SemiColon);
		ADDKEY(COMMA, Comma);
		ADDKEY(PERIOD, Period);
		ADDKEY(APOSTROPHE, Quote);
		ADDKEY(SLASH, Slash);
		ADDKEY(BACKSLASH, BackSlash); 
		ADDKEY(GRAVE_ACCENT, Tilde);
		ADDKEY(EQUAL, Equal);
		ADDKEY(MINUS, Subtract);
		ADDKEY(SPACE, Space);
		ADDKEY(ENTER, Return);
		ADDKEY(BACKSPACE, BackSpace);
		ADDKEY(TAB, Tab);
		ADDKEY(PAGE_UP, PageUp);
		ADDKEY(PAGE_DOWN, PageDown);
		ADDKEY(END, End);
		ADDKEY(HOME, Home);
		ADDKEY(INSERT, Insert);
		ADDKEY(DELETE, Delete);
		ADDKEY(KP_ADD, Add);
		ADDKEY(KP_SUBTRACT, Subtract);
		ADDKEY(KP_MULTIPLY, Multiply);
		ADDKEY(KP_DIVIDE, Divide);
		ADDKEY(LEFT, Left);
		ADDKEY(RIGHT, Right);
		ADDKEY(UP, Up);
		ADDKEY(DOWN, Down);
		ADDKEY(KP_0, Numpad0);
		ADDKEY(KP_1, Numpad1);
		ADDKEY(KP_2, Numpad2);
		ADDKEY(KP_3, Numpad3);
		ADDKEY(KP_4, Numpad4);
		ADDKEY(KP_5, Numpad5);
		ADDKEY(KP_6, Numpad6);
		ADDKEY(KP_7, Numpad7);
		ADDKEY(KP_8, Numpad8);
		ADDKEY(KP_9, Numpad9);
		ADDKEY(F1, F1);
		ADDKEY(F2, F2);
		ADDKEY(F3, F3);
		ADDKEY(F4, F4);
		ADDKEY(F5, F5);
		ADDKEY(F6, F6);
		ADDKEY(F7, F7);
		ADDKEY(F8, F8);
		ADDKEY(F9, F9);
		ADDKEY(F10, F10);
		ADDKEY(F11, F11);
		ADDKEY(F12, F12);
		ADDKEY(F13, F13);
		ADDKEY(F14, F14);
		ADDKEY(F15, F15);
		ADDKEY(PAUSE, Pause);
#undef ADDKEY
	}
	return Keyboard::Key::Invalid;
}

bool Magma::GLFWWindow::DerivedIsOpen()
{
	return m_glfwWindow != nullptr && !glfwWindowShouldClose(m_glfwWindow);
}

void Magma::GLFWWindow::DerivedOpen()
{
	// Open window
	switch (m_windowMode)
	{
	case Window::Mode::Windowed:
		m_glfwWindow = glfwCreateWindow(m_windowSize.x, m_windowSize.y, m_windowTitle.c_str(), NULL, NULL);
		break;
	case Window::Mode::Fullscreen:
		m_glfwWindow = glfwCreateWindow(m_windowSize.x, m_windowSize.y, m_windowTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
		break;
	case Window::Mode::Borderless:
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		m_windowSize.x = mode->width;
		m_windowSize.y = mode->height;

		m_glfwWindow = glfwCreateWindow(m_windowSize.x, m_windowSize.y, m_windowTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
		break;
	}
	case Window::Mode::Splashscreen:
		MAGMA_ERROR("Failed to open GLFWWindow, GLFW doesn't support splash screens");
		break;
	case Window::Mode::Invalid:
		MAGMA_ERROR("Failed to open GLFWWindow, window mode is Invalid");
		break;
	}

	this->SetMouseCursorVisible(true);

	glfwSetKeyCallback(m_glfwWindow, GLFWKeyCallback);
	glfwSetMouseButtonCallback(m_glfwWindow, GLFWMouseButtonCallback);
	glfwSetScrollCallback(m_glfwWindow, GLFWMouseScrollCallback);
	glfwSetCursorPosCallback(m_glfwWindow, GLFWCursorPosCallback);
	glfwSetCursorEnterCallback(m_glfwWindow, GLFWCursorEnterCallback);
	glfwSetWindowSizeCallback(m_glfwWindow, GLFWResizeCallback);
	glfwSetWindowFocusCallback(m_glfwWindow, GLFWFocusCallback);
	glfwSetWindowCloseCallback(m_glfwWindow, GLFWCloseCallback);
}

void Magma::GLFWWindow::DerivedClose()
{
	glfwSetWindowShouldClose(m_glfwWindow, true);
}

void Magma::GLFWWindow::DerivedResize()
{
	glfwSetWindowSize(m_glfwWindow, m_windowSize.x, m_windowSize.y);
}

void Magma::GLFWWindow::DerivedSetTitle()
{
	glfwSetWindowTitle(m_glfwWindow, m_windowTitle.c_str());
}

void Magma::GLFWWindow::DerivedSetMode()
{
	// Restart window again
	this->DerivedClose();
	this->DerivedOpen();
}

void Magma::GLFWWindow::SetVSyncEnabled(bool vsyncEnabled)
{
	if (vsyncEnabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

bool Magma::GLFWWindow::DerivedPollEvent(Magma::UIEvent & event)
{
	glfwPollEvents();

	if (m_eventQueue.empty())
		return false;
	else
	{
		event = m_eventQueue.front();
		m_eventQueue.pop();
		return true;
	}
}

void Magma::GLFWWindow::DerivedDisplay()
{
	glfwSwapBuffers(m_glfwWindow);
}

glm::vec2 Magma::GLFWWindow::GetMousePosition()
{
	double x = -1.0, y = -1.0;
	glfwGetCursorPos(m_glfwWindow, &x, &y);
	return glm::vec2(x, y);
}

void Magma::GLFWWindow::SetMousePosition(glm::vec2 mousePosition)
{
	glfwSetCursorPos(m_glfwWindow, mousePosition.x, mousePosition.y);
}

bool Magma::GLFWWindow::IsKeyboardKeyPressed(Keyboard::Key key)
{
	if (key == Keyboard::Key::Invalid)
		return false;
	return m_keyboardKeyStates[static_cast<size_t>(key)];
}

bool Magma::GLFWWindow::IsMouseCursorVisible()
{
	return m_mouseCursorVisible;
}

bool Magma::GLFWWindow::IsMouseButtonPressed(Mouse::Button button)
{
	if (button == Mouse::Button::Invalid)
		return false;
	return m_mouseButtonStates[static_cast<size_t>(button)];
}

void Magma::GLFWWindow::SetMouseCursorVisible(bool visible)
{
	m_mouseCursorVisible = visible;
	if (!m_mouseCursorVisible)
		glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	else
		glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
