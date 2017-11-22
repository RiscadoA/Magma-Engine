#pragma once

#include "..\Utils\Math.hpp"

namespace Magma
{
	/// <summary>
	///		Class used to give access to the real-time state of the mouse
	/// </summary>
	class Mouse
	{
	public:
		/// <summary>
		///		Mouse buttons
		/// </summary>
		enum class Button : int32_t
		{
			Invalid = -1,	// Invalid mouse button

			Left = 0,		// Left mouse button
			Right,			// Right mouse button
			Middle,			// Middle mouse button
			Extra1,			// First extra mouse button
			Extra2,			// Second extra mouse button

			Count			// Total number of mouse buttons
		};

		/// <summary>
		///		Mouse wheels
		/// </summary>
		enum class Wheel : int32_t
		{
			Invalid = -1,	// Invalid mouse wheel

			Vertical = 0,	// Vertical mouse wheel
			Horizontal,		// Horizontal mouse wheel

			Count			// Total number of mouse wheels
		};

		/// <summary>
		///		Checks if mouse cursor is visible
		/// </summary>
		/// <returns>True if mouse cursor is visible, otherwise false</returns>
		static bool IsVisible();

		/// <summary>
		///		Sets the mouse cursor visibility
		/// </summary>
		/// <param name="visible">If true the mouse cursor is set to visible, otherwise to invisible</param>
		static void SetVisible(bool visible);

		/// <summary>
		///		Checks if a mouse button is pressed
		/// </summary>
		/// <param name="button">Button to check</param>
		/// <returns>True if button is pressed, false otherwise</returns>
		static bool IsButtonPressed(Mouse::Button button);

		/// <summary>
		///		Get the current position of the mouse in the active <see cref="Magma::Window"/>
		/// </summary>
		static glm::vec2 GetPosition();

		/// <summary>
		///		Sets the current position of the mouse in the active <see cref="Magma::Window"/>
		/// </summary>
		/// <param name="position">New mouse position</param>
		static void SetPosition(glm::vec2 position);
	};
}
