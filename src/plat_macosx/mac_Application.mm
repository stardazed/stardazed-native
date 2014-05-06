// ------------------------------------------------------------------
// Mac OS X Application Scaffolding - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/Application.hpp"
#include "system/Logging.hpp"
#include "input/Keyboard.hpp"
#include "mac_VKeyCodes.hpp"

#import <AppKit/AppKit.h>
#include <memory>
#include <array>
#include <algorithm>


@interface SDAppDelegate : NSObject<NSApplicationDelegate> {
}
@property (nonatomic, strong) NSArray *xibObjects;
@end

@implementation SDAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification*)notification {
	stardazed::log("applicationDidFinishLaunching");
}
@end



namespace stardazed {

namespace {
	// static data
	bool quit = false;
	std::array<input::Key, 512> keyTransTable;


	// local functions
	void changeToResourcesDirectory() {
		const char *resourcePath = [[[NSBundle mainBundle] resourcePath] UTF8String];
		chdir(resourcePath);
	}


	void initApplication() {
		[NSApplication sharedApplication];
		[NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];

		auto appDelegate = [[SDAppDelegate alloc] init];
		[NSApp setDelegate: appDelegate];

		NSArray *tlo;
		[[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:appDelegate topLevelObjects: &tlo];
		appDelegate.xibObjects = tlo;
		
		changeToResourcesDirectory();

		[NSApp finishLaunching];
	}
	
	
	void buildKeyTranslationTable() {
		using namespace input;

		std::fill(keyTransTable.begin(), keyTransTable.end(), input::Key::Unknown);
		auto& ktt = keyTransTable;
		
		ktt[kVK_ANSI_A]      = Key::A;          ktt[kVK_ANSI_B]      = Key::B;
		ktt[kVK_ANSI_C]      = Key::C;          ktt[kVK_ANSI_D]      = Key::D;
		ktt[kVK_ANSI_E]      = Key::E;          ktt[kVK_ANSI_F]      = Key::F;
		ktt[kVK_ANSI_G]      = Key::G;          ktt[kVK_ANSI_H]      = Key::H;
		ktt[kVK_ANSI_I]      = Key::I;          ktt[kVK_ANSI_J]      = Key::J;
		ktt[kVK_ANSI_K]      = Key::K;          ktt[kVK_ANSI_L]      = Key::L;
		ktt[kVK_ANSI_M]      = Key::M;          ktt[kVK_ANSI_N]      = Key::N;
		ktt[kVK_ANSI_O]      = Key::O;          ktt[kVK_ANSI_P]      = Key::P;
		ktt[kVK_ANSI_Q]      = Key::Q;          ktt[kVK_ANSI_R]      = Key::R;
		ktt[kVK_ANSI_S]      = Key::S;          ktt[kVK_ANSI_T]      = Key::T;
		ktt[kVK_ANSI_U]      = Key::U;          ktt[kVK_ANSI_V]      = Key::V;
		ktt[kVK_ANSI_W]      = Key::W;          ktt[kVK_ANSI_X]      = Key::X;
		ktt[kVK_ANSI_Y]      = Key::Y;          ktt[kVK_ANSI_Z]      = Key::Z;

		ktt[kVK_ANSI_0]      = Key::Num0;       ktt[kVK_ANSI_1]      = Key::Num1;
		ktt[kVK_ANSI_2]      = Key::Num2;       ktt[kVK_ANSI_3]      = Key::Num3;
		ktt[kVK_ANSI_4]      = Key::Num4;       ktt[kVK_ANSI_5]      = Key::Num5;
		ktt[kVK_ANSI_6]      = Key::Num6;       ktt[kVK_ANSI_7]      = Key::Num7;
		ktt[kVK_ANSI_8]      = Key::Num8;       ktt[kVK_ANSI_9]      = Key::Num9;

		ktt[kVK_ANSI_Keypad0]      = Key::NumPad0;       ktt[kVK_ANSI_Keypad1]      = Key::NumPad1;
		ktt[kVK_ANSI_Keypad2]      = Key::NumPad2;       ktt[kVK_ANSI_Keypad3]      = Key::NumPad3;
		ktt[kVK_ANSI_Keypad4]      = Key::NumPad4;       ktt[kVK_ANSI_Keypad5]      = Key::NumPad5;
		ktt[kVK_ANSI_Keypad6]      = Key::NumPad6;       ktt[kVK_ANSI_Keypad7]      = Key::NumPad7;
		ktt[kVK_ANSI_Keypad8]      = Key::NumPad8;       ktt[kVK_ANSI_Keypad9]      = Key::NumPad9;
		
		ktt[kVK_ANSI_KeypadDecimal]      = Key::NumPadDecimal;       ktt[kVK_ANSI_KeypadEnter]      = Key::NumPadEnter;
		ktt[kVK_ANSI_KeypadPlus]      = Key::NumPadPlus;       ktt[kVK_ANSI_KeypadMinus]      = Key::NumPadMinus;
		ktt[kVK_ANSI_KeypadMultiply]      = Key::NumPadMultiply;       ktt[kVK_ANSI_KeypadDivide]      = Key::NumPadDivide;
		ktt[kVK_ANSI_KeypadEquals]      = Key::NumPadEquals;       ktt[kVK_ANSI_KeypadClear]      = Key::NumLock;

		ktt[kVK_Space]      = Key::Space;       ktt[kVK_Delete]      = Key::Backspace;
		ktt[kVK_Tab]      = Key::Tab;       ktt[kVK_Return]      = Key::Return;

		ktt[kVK_Help]      = Key::Insert;       ktt[kVK_ForwardDelete]      = Key::Delete;
		ktt[kVK_Home]      = Key::Home;       ktt[kVK_End]      = Key::End;
		ktt[kVK_PageUp]      = Key::PageUp;       ktt[kVK_PageDown]      = Key::PageDown;

		ktt[kVK_UpArrow]      = Key::ArrowUp;       ktt[kVK_DownArrow]      = Key::ArrowDown;
		ktt[kVK_LeftArrow]      = Key::ArrowLeft;       ktt[kVK_RightArrow]      = Key::ArrowRight;

		
	}
} // ns anon



void quitNow() {
	quit = true;
}
	
	




Application::Application() {
	initApplication();
	buildKeyTranslationTable();
}


bool Application::shouldQuit() {
	return quit;
}


void Application::yieldSystem() {
	@autoreleasepool {
		NSEvent *ev;
		
		do {
			ev = [NSApp nextEventMatchingMask: NSAnyEventMask
									untilDate: nil
									   inMode: NSDefaultRunLoopMode
									  dequeue: YES];
			if (ev) {
				[NSApp sendEvent: ev];
				
				if ([ev type] == NSKeyDown)
					NSLog(@"VK: %i, mods: %x", [ev keyCode], (uint32_t)[ev modifierFlags]);
			}
		} while (ev);
	}
}

} // stardazed namespace
