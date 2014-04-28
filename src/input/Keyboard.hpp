// ------------------------------------------------------------------
// input::Keyboard - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_INPUT_KEYBOARD_H
#define SD_INPUT_KEYBOARD_H

#include <string>

namespace stardazed {
namespace input {


// Key names based on western keyboard layout
enum class Key {
	Unknown,

	A, B, C, D, E, F, G, H, I,
	J, K, L, M, N, O, P, Q, R,
	S, T, U, V, W, X, Y, Z,

	Num1, Num2, Num3, Num4, Num5,
	Num6, Num7, Num8, Num9, Num0,

	NumPad1, NumPad2, NumPad3, NumPad4, NumPad5,
	NumPad6, NumPad7, NumPad8, NumPad9, NumPad0,
	NumPadDecimal, NumPadEnter,
	NumPadPlus, NumPadMinus, NumPadMultiply, NumPadDivide,
	NumPadEquals, // Mac only
	NumLock,
	
	Space, Backspace, Tab, Return,

	Insert, Delete, Home, End, PageUp, PageDown,
	ArrowUp, ArrowLeft, ArrowDown, ArrowRight,
	
	Minus, Plus,
	OpenBracket, CloseBracket,
	Semicolon, Quote, Backquote,
	Backslash, Slash,
	Comma, Period,
	Paragraph, // Mac only

	Escape,
	F1, F2, F3, F4, F5, F6,
	F7, F8, F9, F10, F11, F12,
	F13, F14, F15, // aka PrScrn, ScrollLock and Pause
	F16, F17, F18, F19, // Mac only
	
	CapsLock,
	LeftShift, RightShift,
	LeftControl, RightControl,
	LeftAlt, RightAlt,
	LeftSystem, RightSystem,
	Context // Win only
};


class Keyboard {
public:
	bool isPressed(Key kc);
	std::string nameForKey(Key k);
};

	

} // ns input
} // ns stardazed

#endif
