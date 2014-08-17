// ------------------------------------------------------------------
// MacInputContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/Logging.hpp"
#include "plat_macosx/mac_InputContext.hpp"
#include "plat_macosx/mac_VKeyCodes.hpp"

#import <AppKit/AppKit.h>
#include <algorithm>

namespace stardazed {
namespace input {


MacInputContext::MacInputContext() {
	buildKeyTranslationTable();
}


bool MacInputContext::isKeyPressed(Key) {
	return false;
}


void MacInputContext::processSystemEvents() {
	@autoreleasepool {
		NSEvent* ev;

		do {
			ev = [NSApp nextEventMatchingMask: NSAnyEventMask
									untilDate: nil
									   inMode: NSDefaultRunLoopMode
									  dequeue: YES];
			if (ev) {
				[NSApp sendEvent: ev];

				if ([ev type] == NSKeyDown)
					NSLog(@"VK: %x, SDK: %hx", [ev keyCode], keyTransTable_[[ev keyCode]]);
			}
		} while (ev);
	}
}


void MacInputContext::buildKeyTranslationTable() {
	std::fill(keyTransTable_.begin(), keyTransTable_.end(), Key::Unknown);
	auto& ktt = keyTransTable_;
	
	// Letters
	ktt[kVK_ANSI_A]              = Key::A;                     ktt[kVK_ANSI_B]            = Key::B;
	ktt[kVK_ANSI_C]              = Key::C;                     ktt[kVK_ANSI_D]            = Key::D;
	ktt[kVK_ANSI_E]              = Key::E;                     ktt[kVK_ANSI_F]            = Key::F;
	ktt[kVK_ANSI_G]              = Key::G;                     ktt[kVK_ANSI_H]            = Key::H;
	ktt[kVK_ANSI_I]              = Key::I;                     ktt[kVK_ANSI_J]            = Key::J;
	ktt[kVK_ANSI_K]              = Key::K;                     ktt[kVK_ANSI_L]            = Key::L;
	ktt[kVK_ANSI_M]              = Key::M;                     ktt[kVK_ANSI_N]            = Key::N;
	ktt[kVK_ANSI_O]              = Key::O;                     ktt[kVK_ANSI_P]            = Key::P;
	ktt[kVK_ANSI_Q]              = Key::Q;                     ktt[kVK_ANSI_R]            = Key::R;
	ktt[kVK_ANSI_S]              = Key::S;                     ktt[kVK_ANSI_T]            = Key::T;
	ktt[kVK_ANSI_U]              = Key::U;                     ktt[kVK_ANSI_V]            = Key::V;
	ktt[kVK_ANSI_W]              = Key::W;                     ktt[kVK_ANSI_X]            = Key::X;
	ktt[kVK_ANSI_Y]              = Key::Y;                     ktt[kVK_ANSI_Z]            = Key::Z;
	
	// Digits
	ktt[kVK_ANSI_0]              = Key::Num0;                  ktt[kVK_ANSI_1]            = Key::Num1;
	ktt[kVK_ANSI_2]              = Key::Num2;                  ktt[kVK_ANSI_3]            = Key::Num3;
	ktt[kVK_ANSI_4]              = Key::Num4;                  ktt[kVK_ANSI_5]            = Key::Num5;
	ktt[kVK_ANSI_6]              = Key::Num6;                  ktt[kVK_ANSI_7]            = Key::Num7;
	ktt[kVK_ANSI_8]              = Key::Num8;                  ktt[kVK_ANSI_9]            = Key::Num9;
	
	// Numeric Keypad
	ktt[kVK_ANSI_Keypad0]        = Key::NumPad0;               ktt[kVK_ANSI_Keypad1]      = Key::NumPad1;
	ktt[kVK_ANSI_Keypad2]        = Key::NumPad2;               ktt[kVK_ANSI_Keypad3]      = Key::NumPad3;
	ktt[kVK_ANSI_Keypad4]        = Key::NumPad4;               ktt[kVK_ANSI_Keypad5]      = Key::NumPad5;
	ktt[kVK_ANSI_Keypad6]        = Key::NumPad6;               ktt[kVK_ANSI_Keypad7]      = Key::NumPad7;
	ktt[kVK_ANSI_Keypad8]        = Key::NumPad8;               ktt[kVK_ANSI_Keypad9]      = Key::NumPad9;
	
	ktt[kVK_ANSI_KeypadDecimal]  = Key::NumPadDecimal;         ktt[kVK_ANSI_KeypadEnter]  = Key::NumPadEnter;
	ktt[kVK_ANSI_KeypadPlus]     = Key::NumPadPlus;            ktt[kVK_ANSI_KeypadMinus]  = Key::NumPadMinus;
	ktt[kVK_ANSI_KeypadMultiply] = Key::NumPadMultiply;        ktt[kVK_ANSI_KeypadDivide] = Key::NumPadDivide;
	ktt[kVK_ANSI_KeypadEquals]   = Key::NumPadEquals;          ktt[kVK_ANSI_KeypadClear]  = Key::NumLock;
	
	// Whitespace
	ktt[kVK_Space]               = Key::Space;                 ktt[kVK_Delete]            = Key::Backspace;
	ktt[kVK_Tab]                 = Key::Tab;                   ktt[kVK_Return]            = Key::Return;
	
	// Page navigation keys
	ktt[kVK_Help]                = Key::Insert;                ktt[kVK_ForwardDelete]     = Key::Delete;
	ktt[kVK_Home]                = Key::Home;                  ktt[kVK_End]               = Key::End;
	ktt[kVK_PageUp]              = Key::PageUp;                ktt[kVK_PageDown]          = Key::PageDown;
	
	// Arrow keys
	ktt[kVK_UpArrow]             = Key::ArrowUp;               ktt[kVK_DownArrow]         = Key::ArrowDown;
	ktt[kVK_LeftArrow]           = Key::ArrowLeft;             ktt[kVK_RightArrow]        = Key::ArrowRight;
	
	// Non-alphanumeric characters
	ktt[kVK_ANSI_Minus]          = Key::Minus;                 ktt[kVK_ANSI_Equal]        = Key::Plus;
	ktt[kVK_ANSI_LeftBracket]    = Key::OpenBracket;           ktt[kVK_ANSI_RightBracket] = Key::CloseBracket;
	ktt[kVK_ANSI_Semicolon]      = Key::Semicolon;             ktt[kVK_ANSI_Quote]        = Key::Quote;
	ktt[kVK_ANSI_Grave]          = Key::Backquote;             ktt[kVK_ANSI_Backslash]    = Key::Backslash;
	ktt[kVK_ANSI_Slash]          = Key::Slash;                 ktt[kVK_ANSI_Comma]        = Key::Comma;
	ktt[kVK_ANSI_Period]         = Key::Period;                ktt[kVK_ISO_Section]       = Key::Section;
	
	// Function keys and Escape
	ktt[kVK_Escape]              = Key::Escape;                ktt[kVK_F1]                = Key::F1;
	ktt[kVK_F2]                  = Key::F2;                    ktt[kVK_F3]                = Key::F3;
	ktt[kVK_F4]                  = Key::F4;                    ktt[kVK_F5]                = Key::F5;
	ktt[kVK_F6]                  = Key::F6;                    ktt[kVK_F7]                = Key::F7;
	ktt[kVK_F8]                  = Key::F8;                    ktt[kVK_F9]                = Key::F9;
	ktt[kVK_F10]                 = Key::F10;                   ktt[kVK_F11]               = Key::F11;
	ktt[kVK_F12]                 = Key::F12;                   ktt[kVK_F13]               = Key::F13;
	ktt[kVK_F14]                 = Key::F14;                   ktt[kVK_F15]               = Key::F15;
	ktt[kVK_F16]                 = Key::F16;                   ktt[kVK_F17]               = Key::F17;
	ktt[kVK_F18]                 = Key::F18;                   ktt[kVK_F19]               = Key::F19;
	
	// Modifier keys
	ktt[kVK_CapsLock]            = Key::CapsLock;
	ktt[kVK_Shift]               = Key::LeftShift;             ktt[kVK_RightShift]        = Key::RightShift;
	ktt[kVK_Control]             = Key::LeftControl;           ktt[kVK_RightControl]      = Key::RightControl;
	ktt[kVK_Option]              = Key::LeftAlt;               ktt[kVK_RightOption]       = Key::RightAlt;
	ktt[kVK_Command]             = Key::LeftMeta;              ktt[kVK_SD_RightCommand]   = Key::RightMeta;
}

	
} // ns input
} // ns stardazed
