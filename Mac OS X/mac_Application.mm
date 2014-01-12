// ------------------------------------------------------------------
// Mac OS X Application Scaffolding - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "Application.h"
#include "../include/RenderTarget.h"
#import <AppKit/AppKit.h>
#include <memory>


@interface SDAppDelegate : NSObject<NSApplicationDelegate> {
}
@property (nonatomic, strong) NSArray *xibObjects;
@end

@implementation SDAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification*)notification {
	NSLog(@"applicationDidFinishLaunching");
}
@end



namespace stardazed {

namespace {
	void initApplication() {
		[NSApplication sharedApplication];
		[NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];

		auto appDelegate = [[SDAppDelegate alloc] init];
		[NSApp setDelegate: appDelegate];

		NSArray *tlo;
		[[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:appDelegate topLevelObjects: &tlo];
		appDelegate.xibObjects = tlo;

		[NSApp finishLaunching];
	}

	std::unique_ptr<RenderTarget> renderTarget;
	bool quit = false;
} // anonymous namespace


void quitNow() {
	quit = true;
}


Application::Application() {
	initApplication();

	RenderTargetOptions options;
	renderTarget = std::make_unique<RenderTarget>(options);
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
			if (ev)
				[NSApp sendEvent: ev];
		} while (ev);
		
		renderTarget->swap();
	}
}

} // stardazed namespace
