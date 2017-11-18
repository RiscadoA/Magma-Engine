#include "Mouse.hpp"

#include "..\Utils\Utils.hpp"

#include "Window.hpp"

bool Magma::Mouse::IsVisible()
{
	Magma::Window* activeWindow = Magma::Window::GetActive();
	if (activeWindow == nullptr)
	{
		MAGMA_ERROR("Failed to retrieve mouse cursor visibility because there is no active window");
		return false;
	}
	return activeWindow->IsMouseCursorVisible();
}

void Magma::Mouse::SetVisible(bool visible)
{
	Magma::Window* activeWindow = Magma::Window::GetActive();
	if (activeWindow == nullptr)
	{
		MAGMA_ERROR("Failed to set mouse cursor visibility because there is no active window");
		return;
	}
	activeWindow->SetMouseCursorVisible(visible);
}

bool Magma::Mouse::IsButtonPressed(Mouse::Button button)
{
	Magma::Window* activeWindow = Magma::Window::GetActive();
	if (activeWindow == nullptr)
	{
		MAGMA_ERROR("Failed to check if mouse button is pressed because there is no active window");
		return false;
	}
	return activeWindow->IsMouseButtonPressed(button);
}

glm::vec2 Magma::Mouse::GetPosition()
{
	Magma::Window* activeWindow = Magma::Window::GetActive();
	if (activeWindow == nullptr)
	{
		MAGMA_ERROR("Failed to retrieve mouse position because there is no active window");
		return glm::vec2(0.0f, 0.0f);
	}
	return activeWindow->GetMousePosition();
}

void Magma::Mouse::SetPosition(glm::vec2 position)
{
	Magma::Window* activeWindow = Magma::Window::GetActive();
	if (activeWindow == nullptr)
	{
		MAGMA_ERROR("Failed to retrieve mouse position because there is no active window");
		return;
	}
	activeWindow->SetMousePosition(position);
}
