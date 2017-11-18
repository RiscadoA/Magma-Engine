#include "Keyboard.hpp"
#include "Window.hpp"

#include "..\Utils\Utils.hpp"

bool Magma::Keyboard::IsKeyPressed(Keyboard::Key key)
{
	Magma::Window* activeWindow = Magma::Window::GetActive();
	if (activeWindow == nullptr)
	{
		MAGMA_ERROR("Failed to check if mouse button is pressed because there is no active window");
		return false;
	}
	return activeWindow->IsKeyboardKeyPressed(key);
}
