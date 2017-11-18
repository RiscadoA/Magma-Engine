#pragma once

#include <cstdint>

namespace Magma
{
	/// <summary>
	///		Class used to give access to the real-time state of the keyboard
	/// </summary>
	class Keyboard
	{
	public:
		/// <summary>
		///		Keyboard key codes
		/// </summary>
		enum class Key : int32_t
		{
			Invalid = -1,	// Invalid key

			A = 0,			// A key
			B,				// B key
			C,				// C key
			D,				// D key
			E,				// E key
			F,				// F key
			G,				// G key
			H,				// H key
			I,				// I key
			J,				// J key
			K,				// K key
			L,				// L key
			M,				// M key
			N,				// N key
			O,				// O key
			P,				// P key
			Q,				// Q key
			R,				// R key
			S,				// S key
			T,				// T key
			U,				// U key
			V,				// V key
			W,				// W key
			X,				// X key
			Y,				// Y key
			Z,				// Z key
			Num0,			// Number 0 key (top of the keyboard)
			Num1,			// Number 1 key (top of the keyboard)
			Num2,			// Number 2 key (top of the keyboard)
			Num3,			// Number 3 key (top of the keyboard)
			Num4,			// Number 4 key (top of the keyboard)
			Num5,			// Number 5 key (top of the keyboard)
			Num6,			// Number 6 key (top of the keyboard)
			Num7,			// Number 7 key (top of the keyboard)
			Num8,			// Number 8 key (top of the keyboard)
			Num9,			// Number 9 key (top of the keyboard)
			Escape,			// Escape key
			LControl,		// Left control key
			LShift,			// Left shift key
			LAlt,			// Left alt key
			LSystem,		// Left system key
			RControl,		// Right control key
			RShift,			// Right shitft key
			RAlt,			// Right alt key
			RSystem,		// Right system key
			Menu,			// Menu key
			LBracket,		// Left bracket key
			RBracket,		// Right bracket key
			SemiColon,		// Semi colon key
			Comma,			// Comma key
			Period,			// Period key
			Quote,			// Quote key
			Slash,			// Slash key
			BackSlash,		// Back slash key
			Tilde,			// Tilde key
			Equal,			// Equal key
			Dash,			// Dash key
			Space,			// Space key
			Return,			// Return key
			BackSpace,		// Back space key
			Tab,			// Tab key
			PageUp,			// Page up key
			PageDown,		// Page down key
			End,			// End key
			Home,			// Home key
			Insert,			// Insert key
			Delete,			// Delete key
			Add,			// Add key
			Subtract,		// Subtract key
			Multiply,		// Multiply key
			Divide,			// Divide key
			Left,			// Left arrow key
			Right,			// Right arrow key
			Up,				// Up arrow key
			Down,			// Down arrow key
			Numpad0,		// Number 0 key (numpad)
			Numpad1,		// Number 1 key (numpad)
			Numpad2,		// Number 2 key (numpad)
			Numpad3,		// Number 3 key (numpad)
			Numpad4,		// Number 4 key (numpad)
			Numpad5,		// Number 5 key (numpad)
			Numpad6,		// Number 6 key (numpad)
			Numpad7,		// Number 7 key (numpad)
			Numpad8,		// Number 8 key (numpad)
			Numpad9,		// Number 9 key (numpad)
			F1,				// F1 key
			F2,				// F2 key
			F3,				// F3 key
			F4,				// F4 key
			F5,				// F5 key
			F6,				// F6 key
			F7,				// F7 key
			F8,				// F8 key
			F9,				// F9 key
			F10,			// F10 key
			F11,			// F11 key
			F12,			// F12 key
			F13,			// F13 key
			F14,			// F14 key
			F15,			// F15 key
			Pause,			// Puase key

			Count,			// Total number of key codes supported
		};

		/// <summary>
		///		Checks if a keyboard key is pressed
		/// </summary>
		/// <param name="key">Keyboard key code to check</param>
		/// <returns>True if keyboard key is pressed, false otherwise</returns>
		static bool IsKeyPressed(Keyboard::Key key);
	};
}
