// ------------------------------------------------------------------
// io::input - stardazed
// (c) 2016 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_IO_INPUT_H
#define SD_IO_INPUT_H

#include "system/Config.hpp"
#include "container/MultiArrayBuffer.hpp"

namespace stardazed {
namespace io {


struct ButtonState {
	bool8 endedDown;
	uint8 halfTransitionCount;
};


struct DirectionalPad {
	ButtonState left, right, up, down;
};


struct Stick {
	float posX, posY;
};


inline Stick stickFromDPad(const DirectionalPad& dPad) {
	Stick stick {};
	
	if (dPad.left.endedDown)
		stick.posX = -1;
	else if (dPad.right.endedDown)
		stick.posX =  1;
	
	if (dPad.up.endedDown)
		stick.posY = -1;
	else if (dPad.down.endedDown)
		stick.posY =  1;
	
	return stick;
}


// Key names based on ANSI/ISO keyboard layout
enum class Key : uint16 {
	None,
	
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
	ArrowUp, ArrowDown, ArrowLeft, ArrowRight,
	
	Minus, Plus,
	OpenBracket, CloseBracket,
	Semicolon, Quote, Backquote,
	Backslash, Slash,
	Comma, Period,
	Section, // Mac ISO only
	
	Escape,
	F1, F2, F3, F4, F5, F6,
	F7, F8, F9, F10, F11, F12,
	F13, F14, F15, // aka PrintScreen, ScrollLock and Pause/Break
	F16, F17, F18, F19, // Mac full-width only
	
	CapsLock,
	LeftShift, RightShift,
	LeftControl, RightControl,
	LeftAlt, RightAlt,
	LeftMeta, RightMeta,
	Context // Win only
};


class Keyboard {
	container::MultiArrayBuffer<
		bool8, // down
		uint8  // halfTransitionCount
	> keyData_;

	bool8* downBase_;
	uint8* halfTransBase_;
	
public:
	Keyboard();
	
	ButtonState keyState(Key kc) const;
	bool down(Key kc) const;
	bool pressed(Key kc) const;
	
	uint8 halfTransitions(Key kc) const;
	void resetHalfTransitions();
	
	void clear();
	void keyDownEvent(Key kc);
	void keyUpEvent(Key kc);
};


// global instances
extern Keyboard keyboard;


// global functions (platform-dependent)
void update();


} // ns io
} // ns stardazed

#endif
