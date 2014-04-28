// ------------------------------------------------------------------
// Mac OS X Application Scaffolding - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/Application.hpp"
#include "system/Logging.hpp"
#include "system/RenderTarget.hpp"

#import <AppKit/AppKit.h>
#include <memory>


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

	bool quit = false;
} // ns anon



void quitNow() {
	quit = true;
}



Application::Application() {
	initApplication();
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
				
//				NSLog(@"Event: %lu, mods: %x", [ev type], (uint32_t)[ev modifierFlags]);
			}
		} while (ev);
	}
}

} // stardazed namespace
