#pragma once

#include "..\Utils\Serializable.hpp"

#include "Mouse.hpp"
#include "Keyboard.hpp"

namespace Magma
{
	/// <summary>
	///		UI Events data structs
	/// </summary>
	namespace UIEventData
	{
		/// <summary>
		///		Size UI event data
		/// </summary>
		struct SizeUIEvent
		{
			/// <summary>
			///		Width
			/// </summary>
			uint32_t width;
			/// <summary>
			///		Height
			/// </summary>
			uint32_t height;
		};

		/// <summary>
		///		Key UI event data
		/// </summary>
		struct KeyUIEvent
		{
			/// <summary>
			///		Key code
			/// </summary>
			Keyboard::Key code;
			/// <summary>
			///		Is shift pressed?
			/// </summary>
			bool shift;
			/// <summary>
			///		Is control pressed?
			/// </summary>
			bool control;
			/// <summary>
			///		Is alt pressed?
			/// </summary>
			bool alt;
		};

		/// <summary>
		///		Text UI event data
		/// </summary>
		struct TextUIEvent
		{
			/// <summary>
			///		Character unicode
			/// </summary>
			uint32_t unicode;
		};

		/// <summary>
		///		Mouse Movement UI event data
		/// </summary>
		struct MouseMoveUIEvent
		{
			/// <summary>
			///		Mouse pointer X coordinate
			/// </summary>
			int32_t x;
			/// <summary>
			///		Mouse pointer Y coordinate
			/// </summary>
			int32_t y;
		};

		/// <summary>
		///		Mouse Button UI event data
		/// </summary>
		struct MouseButtonUIEvent
		{
			/// <summary>
			///		Mouse button
			/// </summary>
			Mouse::Button button;
			/// <summary>
			///		Mouse pointer X coordinate
			/// </summary>
			int32_t x;
			/// <summary>
			///		Mouse pointer Y coordinate
			/// </summary>
			int32_t y;
		};

		/// <summary>
		///		Mouse Wheel Scroll UI event data
		/// </summary>
		struct MouseWheelScrollUIEvent
		{
			/// <summary>
			///		Which wheel was scrolled
			/// </summary>
			Mouse::Wheel wheel;
			/// <summary>
			///		Wheel offset (positive is up/left, negative is down/right)
			/// </summary>
			float delta;
			/// <summary>
			///		X position of the mouse pointer, relative to the left of the owner window
			/// </summary>
			int32_t x;
			/// <summary>
			///		Y position of the mouse pointer, relative to the top of the owner window
			/// </summary>
			int32_t y;
		};
	}

	/// <summary>
	///		Class used to transmit user input event info
	/// </summary>
	class UIEvent : public Serializable
	{
	public:
		/// <summary>
		///		Event types
		/// </summary>
		enum class Type
		{
			Invalid = -1,			// Invalid event type

			Closed = 0,				// When the window is closed
			Resized,				// When the window is resized
			LostFocus,				// When the window losts focus
			GainedFocus,			// When the window gains focus
			TextEntered,			// When text is entered
			KeyPressed,				// When a key is pressed
			KeyReleased,			// When a key is released
			MouseWheelScrolled,		// When a mouse wheel is scrolled
			MouseButtonPressed,		// When a mouse button is pressed
			MouseButtonReleased,	// When a mouse button is released
			MouseMoved,				// When the mouse is moved
			MouseEntered,			// When the mouse enters the window
			MouseLeft,				// When the mouse leaves the window

			Count					// Event type count
		};

		Type type;	// This event type

		union
		{
			UIEventData::SizeUIEvent resized;
			UIEventData::TextUIEvent textEntered;
			UIEventData::KeyUIEvent key, keyPressed, keyReleased;
			UIEventData::MouseWheelScrollUIEvent mouseWheelScrolled;
			UIEventData::MouseButtonUIEvent mouseButton, mouseButtonPressed, mouseButtonReleased;
			UIEventData::MouseMoveUIEvent mouseMoved;
		};

	private:
		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const override;
		virtual void Deserialize(std::istream & stream) override;
	};
}
