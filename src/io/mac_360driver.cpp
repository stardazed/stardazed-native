// ------------------------------------------------------------------
// io::mac_360driver - stardazed
// (c) 2016 by Arthur Langereis
// ------------------------------------------------------------------

#include "io/mac_360driver.hpp"
#include "math/Algorithm.hpp"
#include <algorithm>

namespace stardazed {
namespace io {


// taken from MSDN, defined in XInput.h
static const int X360LeftThumbDeadZone  = 7849;
static const int X360RightThumbDeadZone = 8689;


static void X360ValueCallback(void* context, IOReturn, void*, IOHIDValueRef value) {
	// -- acquire runtime context
	auto controllerCtx = static_cast<ControllerDriverContext*>(context);
	auto& controller = controllerCtx->controller;
	// auto devicesCtx = controllerCtx->devicesContext;

	IOHIDElementRef element = IOHIDValueGetElement(value);
	if (CFGetTypeID(element) != IOHIDElementGetTypeID()) {
		return;
	}
	
	int usage = IOHIDElementGetUsage(element);
	CFIndex elementValue = IOHIDValueGetIntegerValue(value);
	
	if (usage == 50) {
		// Left Trigger
		float normValue = (float)elementValue / 255.f;
		controller.leftTrigger = normValue;
	}
	else if (usage == 53) {
		// Right Trigger
		float normValue = (float)elementValue / 255.f;
		controller.rightTrigger = normValue;
	}
	else if (usage > 47) {
		// Thumb Sticks
		float normValue = elementValue;
		float deadZone = usage < 50 ? X360LeftThumbDeadZone : X360RightThumbDeadZone;
		
		if (normValue < 0) {
			normValue = math::min(0.0f, normValue + deadZone) / (32768.f - deadZone);
		}
		else {
			normValue = math::max(0.0f, normValue - deadZone) / (32767.f - deadZone);
		}
		
		switch (usage) {
			case 48: controller.leftStick.posX  = normValue; break;
			case 49: controller.leftStick.posY  = normValue; break;
			case 51: controller.rightStick.posX = normValue; break;
			case 52: controller.rightStick.posY = normValue; break;
		}
	}
	else {
		// Buttons
		ButtonState* button {};
		
		switch (usage) {
			case  1: button = &controller.A; break;
			case  2: button = &controller.B; break;
			case  3: button = &controller.X; break;
			case  4: button = &controller.Y; break;
			case  5: button = &controller.leftShoulder; break;
			case  6: button = &controller.rightShoulder; break;
			case  7: button = &controller.leftThumb; break;
			case  8: button = &controller.rightThumb; break;
			case  9: button = &controller.start; break;
			case 10: button = &controller.select; break;
			case 11: break; // home button, not supported
			case 12: button = &controller.dPad.up; break;
			case 13: button = &controller.dPad.down; break;
			case 14: button = &controller.dPad.left; break;
			case 15: button = &controller.dPad.right; break;
			default: break;
		}
		
		if (button) {
			++button->halfTransitionCount;
			button->endedDown = elementValue == 1;
		}
	}
}


// This list is generated using the Info.plist of the 360Controller project.
// The values are the product and vendor keys in the higher and lower 16-bits of
// each entry resp. The comment lists the name of the controller.
static uint32 supportedVendorProductKeys[] = {
	0x0E6F0413, // AfterglowGamepad1
	0x07384758, // ArcadeGameStick
	0x11C05506, // BETOPGAMEFORWINDOWS
	0x15E43F10, // BatarangWired
	0x146B0601, // BigBenController
	0xFFFFFFFF, // Chinese-madeXboxController
	0x045E028E, // Controller
	0x0F0D000A, // DOA4Stick
	0x24C6550D, // GEMPADEX
	0x0E6F0401, // GameStopGamepad3
	0x0E6F0301, // GameStopGamepad4
	0x12AB0302, // GamestopGamepad
	0x1BADF901, // GamestopGamepad2
	0x046DF301, // GenericController
	0x14304748, // GuitarHero
	0x1BADF503, // HoriFSVX
	0x0F0D000A, // HoriFightingStickEX2
	0x1BADF505, // HoriFightingStickEX2B
	0x0F0D000D, // HoriFightingStickEX2C
	0x1BADF501, // HoriPadEX2Turbo
	0x0F0D000C, // HoriPadEXTurbo
	0x0F0D0067, // HoriPadOne
	0x0F0D0063, // HoriRAPHayabusaXboxOne
	0x1BADF502, // HoriRAPVXSA
	0x1BAD5500, // HoriUnnamed
	0x1BADFA01, // HoriUnnamedBlueSolo
	0x1BAD0130, // IonDrumRocker
	0x162EBEEF, // JoytekXbox360
	0x12AB0004, // KonamiDancePad
	0x046DC21D, // LogitechF310
	0x046DC21E, // LogitechF510
	0x046DC21F, // LogitechF710
	0x1BADF023, // MLGGamePadforXbox360
	0x1BADF039, // MVC2TEStick
	0x0738B738, // MVC2TEStick2
	0x1BADF019, // MadCatzBrawlStick
	0x1BADF027, // MadCatzFPSPro
	0x1BADF02E, // MadCatzFightPad
	0x1BADF03A, // MadCatzFightStickNeo
	0x1BADF03F, // MadCatzFightStickSoulCaliber
	0x1BADF080, // MadCatzFightStickTE2
	0x07384716, // MadCatzGamepad
	0x1BADF902, // MadCatzGamepad2
	0x1BADF0CA, // MadCatzGamepad3
	0x07384736, // MadCatzMicroConGamepad
	0x07384736, // MadCatzMicroGamepad
	0x1BADF016, // MadCatzPad3
	0x07389871, // MadCatzPortableDrum
	0x07384726, // MadCatzProGamepad
	0x045E0289, // MicrosoftCorp.XboxControllerS
	0x045E0288, // MicrosoftCorp.XboxControllerSHub
	0x045E0285, // MicrosoftX-Boxpad(Japan)
	0x045E0202, // MicrosoftX-Boxpadv1(US)
	0x045E0287, // MicrosoftXboxControllerS
	0x045E02D1, // MicrosoftXboxOneController2013
	0x045E02DD, // MicrosoftXboxOneController2015
	0x1BADF900, // PDPAfterglow
	0x0E6F0113, // PDPAfterglowAX1
	0x0E6F0213, // PDPAfterglowV2
	0x0E6F0147, // PDPMarvelXboxOneController
	0x1BADF903, // PDPTron
	0x1BADF904, // PDPVersusPad
	0x15E43F0A, // PowerAAirflow
	0x15E43F00, // PowerAMiniProEXGreen
	0x24C6531A, // PowerAMiniProEXGreen2
	0x24C65300, // PowerAMiniProEXWhite
	0x24C6541A, // PowerAMiniXboxOne
	0x24C6542A, // PowerASpectraIlluminatedXboxOne
	0x0738BEEF, // QanBaJoystickPlus
	0x0F0D0016, // RAPEXSE
	0x1BADF504, // REALARCADEPROEX
	0x1BADFD01, // RazerOnza
	0x1689FD01, // RazerOnza2
	0x1BADFD00, // RazerOnzaTE
	0x1689FD00, // RazerOnzaTE2
	0x1689FE00, // RazerSabertoothElite
	0x24C65D04, // RazerSabertoothElite2
	0x1430F801, // RedOctaneController
	0x1BAD0003, // RockBandDrums
	0x1BAD0002, // RockBandGuitar
	0x0E6F011F, // RockCandy
	0x0E6F021F, // RockCandyGamepadForXbox360
	0x0E6F0146, // RockCandyGamepadforXboxOne
	0x0F0D000D, // SC4VF5Stick
	0x1BADF028, // SF4FightPad
	0x07384728, // SF4FightPad2
	0x07384718, // SF4StickSE
	0x07384738, // SF4StickTE
	0x1BADF038, // SF4StickTER2
	0x0738F738, // SSF4StickTE
	0x1BADF03D, // SSFIVTEChunLi
	0x0738CB02, // SaitekCB360
	0x16890001, // StrikeController
	0x0E6F0201, // TSZPelican
	0x044FB326, // ThrustMasterGPXGamepad
	0x24C65B02, // ThrustmasterGPXLightback
	0x1BADF906, // XB360MortalKombatFightStick
	0x24C6530A, // Xbox360ProEXController
};


bool X360ControllerDriver::supportsDevice(IOHIDDeviceRef, int vendorID, int productID) {
	uint32 venPro = (vendorID << 16) | productID;

	auto vpEnd = std::end(supportedVendorProductKeys);
	return std::find(std::begin(supportedVendorProductKeys), vpEnd, venPro) != vpEnd;
}


IOHIDValueCallback X360ControllerDriver::callbackForDevice(IOHIDDeviceRef, int, int) {
	return X360ValueCallback;
}

	
} // ns io
} // ns stardazed
