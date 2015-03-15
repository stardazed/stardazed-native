// ------------------------------------------------------------------
// device::mac_DevicesContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "device/mac_DevicesContext.hpp"
#include "device/mac_VKeyCodes.hpp"
#include "device/mac_X360Controller.hpp"
#include "system/Logging.hpp"

#include <algorithm>

#import <AppKit/AppKit.h>

namespace stardazed {
namespace device {


DevicesContext::DevicesContext() {
	buildKeyTranslationTable();
	initControllers();
}


void DevicesContext::handleKeyTransition(Key key, bool isDown) {
	// update keyboard state for polling
	if (isDown)
		keyboard_.press(key);
	else
		keyboard_.release(key);

	// update keyboard controller (#0)
	Controller& controller = controllers_[0].controller;
	KeyboardControllerConfig& config = keyboardControllerConfig_;

	// Directional Pad
	if (key == config.dPadLeft) {
		++controller.dPad.left.halfTransitionCount;
		controller.dPad.left.endedDown = isDown;
	}
	else if (key == config.dPadRight) {
		++controller.dPad.right.halfTransitionCount;
		controller.dPad.right.endedDown = isDown;
	}
	if (key == config.dPadUp) {
		++controller.dPad.up.halfTransitionCount;
		controller.dPad.up.endedDown = isDown;
	}
	else if (key == config.dPadDown) {
		++controller.dPad.down.halfTransitionCount;
		controller.dPad.down.endedDown = isDown;
	}
	
	// Face Buttons
	if (key == config.btnA) {
		++controller.A.halfTransitionCount;
		controller.A.endedDown = isDown;
	}
	if (key == config.btnB) {
		++controller.B.halfTransitionCount;
		controller.B.endedDown = isDown;
	}
	if (key == config.btnX) {
		++controller.X.halfTransitionCount;
		controller.X.endedDown = isDown;
	}
	if (key == config.btnY) {
		++controller.Y.halfTransitionCount;
		controller.Y.endedDown = isDown;
	}

	// Shoulder Buttons
	if (key == config.btnLeftShoulder) {
		++controller.leftShoulder.halfTransitionCount;
		controller.leftShoulder.endedDown = isDown;
	}
	if (key == config.btnRightShoulder) {
		++controller.rightShoulder.halfTransitionCount;
		controller.rightShoulder.endedDown = isDown;
	}

	// Select, Start
	if (key == config.btnSelect) {
		++controller.select.halfTransitionCount;
		controller.select.endedDown = isDown;
	}
	if (key == config.btnStart) {
		++controller.start.halfTransitionCount;
		controller.start.endedDown = isDown;
	}
}


static bool shouldForwardEvent(NSEvent* event) {
	auto evType = [event type];
	bool sysEvent = (evType == NSAppKitDefined) ||
					(evType == NSSystemDefined) ||
					(evType == NSApplicationDefined) ||
					(evType == NSPeriodic);

	return sysEvent || evType == NSLeftMouseDown || ((evType == NSKeyDown) && ([event modifierFlags] & NSCommandKeyMask));
}


void DevicesContext::processSystemEvents() {
	@autoreleasepool {
		NSEvent* ev;
		
		do {
			ev = [NSApp nextEventMatchingMask: NSAnyEventMask
									untilDate: nil
									   inMode: NSDefaultRunLoopMode
									  dequeue: YES];
			if (ev) {
				auto eventType = [ev type];
				switch(eventType) {
					case NSKeyDown:
					case NSKeyUp:
						handleKeyTransition(keyTransTable_[ev.keyCode], eventType == NSKeyDown);
						break;

					case NSLeftMouseDown: break;
					default: break;
				}

				if (shouldForwardEvent(ev)) {
					[NSApp sendEvent: ev];
				}
			}
		} while (ev);
	}
}


void DevicesContext::frame() {
	processSystemEvents();
}


static void hidDeviceRemoved(void* context, IOReturn, void*) {
	auto controllerCtx = static_cast<ControllerDriverContext*>(context);
	controllerCtx->controller.isConnected = false;
}


static void hidDeviceAdded(void* context, IOReturn, void*, IOHIDDeviceRef device) {
	auto devCtx = static_cast<DevicesContext*>(context);

	// -- get the vendor and product ID for quick identification
	auto vendorIDRef = (CFNumberRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey));
	auto productIDRef = (CFNumberRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductIDKey));
	
	int vendorID, productID;
	CFNumberGetValue(vendorIDRef, kCFNumberIntType, &vendorID);
	CFNumberGetValue(productIDRef, kCFNumberIntType, &productID);

	// -- find if we have a registered driver that knows this controller
	auto driverIt = std::find_if(devCtx->driversBegin(), devCtx->driversEnd(),
		[device, vendorID, productID](ControllerDriver* driver){
			return driver->supportsDevice(device, vendorID, productID);
		});

	// -- open device for comms and register callbacks
	if (driverIt != devCtx->driversEnd()) {
		if (IOHIDDeviceOpen(device, kIOHIDOptionsTypeNone) == kIOReturnSuccess) {
			auto& controllerCtx = devCtx->createController();
			auto callback = (*driverIt)->callbackForDevice(device, vendorID, productID);

			IOHIDDeviceRegisterInputValueCallback(device, callback, &controllerCtx);
			IOHIDDeviceRegisterRemovalCallback(device, hidDeviceRemoved, &controllerCtx);
		}
		else {
			// FIXME: can't open device communications, log
		}
	}
	else {
		// FIXME: no suitable driver found, report or log
		sd::log("Unknown Controller/Gamepad, vendorID: ", vendorID, "; productID: ", productID);
	}
}


Controller* DevicesContext::controllerAtIndex(size32 index) {
	if (index >= controllers_.size())
		return nullptr;
	return &controllers_[index].controller;
}


Controller* DevicesContext::keyboardController() {
	return controllerAtIndex(0);
}


ControllerDriverContext& DevicesContext::createController() {
	controllers_.push_back({ {}, this });
	return controllers_.back();
}


void DevicesContext::initControllers() {
	// -- register the controller drivers
	controllerDrivers_.push_back(new X360ControllerDriver{});
	
	// -- create controller #0, the (built-in) keyboard controller
	controllers_.push_back({ {}, this });
	controllers_[0].controller.isConnected = true;
	controllers_[0].controller.isAnalog = false;

	// -- setup the HID manager and callbacks
	hidManager_ = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
	
	NSArray* criteria = @[
		@{	(NSString*)CFSTR(kIOHIDDeviceUsagePageKey):
				[NSNumber numberWithInt:kHIDPage_GenericDesktop],
			(NSString*)CFSTR(kIOHIDDeviceUsageKey):
				[NSNumber numberWithInt:kHIDUsage_GD_Joystick]
		},
		@{	(NSString*)CFSTR(kIOHIDDeviceUsagePageKey):
				[NSNumber numberWithInt:kHIDPage_GenericDesktop],
			(NSString*)CFSTR(kIOHIDDeviceUsageKey):
				[NSNumber numberWithInt:kHIDUsage_GD_GamePad]
		},
		@{	(NSString*)CFSTR(kIOHIDDeviceUsagePageKey):
				[NSNumber numberWithInt:kHIDPage_GenericDesktop],
			(NSString*)CFSTR(kIOHIDDeviceUsageKey):
				[NSNumber numberWithInt:kHIDUsage_GD_MultiAxisController]
		}
	];
	
	IOHIDManagerSetDeviceMatchingMultiple(hidManager_, (__bridge CFArrayRef)criteria);
	IOHIDManagerRegisterDeviceMatchingCallback(hidManager_, hidDeviceAdded, this);
	IOHIDManagerScheduleWithRunLoop(hidManager_, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
}


void DevicesContext::buildKeyTranslationTable() {
	std::fill(keyTransTable_.begin(), keyTransTable_.end(), Key::None);
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


} // ns device
} // ns stardazed
