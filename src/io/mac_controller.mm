// ------------------------------------------------------------------
// io::mac_controller - stardazed
// (c) 2016 by Arthur Langereis
// ------------------------------------------------------------------

#include "io/controller.hpp"
#include "io/mac_360driver.hpp"
#include "system/Logging.hpp"

#include <algorithm>
#include <vector>

#include <IOKit/hid/IOHIDLib.h>
#include <Foundation/Foundation.h>

namespace stardazed {
namespace io {


static void hidDeviceRemoved(void* context, IOReturn, void*) {
	auto controllerCtx = static_cast<ControllerDriverContext*>(context);
	controllerCtx->controller.isConnected = false;
}


static void hidDeviceAdded(void* context, IOReturn, void*, IOHIDDeviceRef device) {
	auto devCtx = static_cast<MacController*>(context);
	
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


MacController::MacController() {
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



uint MacController::count() {
	return static_cast<uint>(controllers_.size());
}


bool MacController::enabled(uint index) {
	return (index < controllers_.size());
}


ControllerState* MacController::state(uint index) {
	if (index >= controllers_.size())
		return nullptr;
	return &controllers_[index].controller;
}


ControllerDriverContext& MacController::createController() {
	controllers_.push_back({ {}, this });
	return controllers_.back();
}


Controller& controller() {
	static MacController* context = nullptr;
	if (! context) {
		context = new MacController{};
	}
	return *context;
}


} // ns io
} // ns stardazed
