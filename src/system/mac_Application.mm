// ------------------------------------------------------------------
// mac_Application - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/mac_Application.hpp"
#include "system/Logging.hpp"

#import <AppKit/AppKit.h>


@interface SDAppDelegate : NSObject<NSApplicationDelegate> {}
@property (nonatomic, strong) NSArray* xibObjects;
@end

@implementation SDAppDelegate
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
	SD_UNUSED_PARAM(sender)
	// since we like control and to keep things sort of similar to non-OS X env
	// we return cancel terminate, but set the global quit flag to true so
	// our own main loop can exit gracefully
	sd::Application::quitNow();
	return NSTerminateCancel;
}

- (void)applicationDidResignActive:(NSNotification *)aNotification {
	SD_UNUSED_PARAM(aNotification)
	sd::Application::setActive(false);
}

- (void)applicationDidBecomeActive:(NSNotification *)aNotification {
	SD_UNUSED_PARAM(aNotification)
	sd::Application::setActive(true);
}
@end


namespace stardazed {


bool Application::quit_ = false;
bool Application::active_ = true;


static void changeToResourcesDirectory() {
	const char *resourcePath = [[[NSBundle mainBundle] resourcePath] UTF8String];
	chdir(resourcePath);
}


void Application::init() {
	auto app = [NSApplication sharedApplication];
	[app setActivationPolicy: NSApplicationActivationPolicyRegular];
	
	auto appDelegate = [[SDAppDelegate alloc] init];
	[app setDelegate: appDelegate];

	// -- install menubar, etc.
	NSArray *tlo;
	[[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:appDelegate topLevelObjects: &tlo];
	appDelegate.xibObjects = tlo;

	// -- allow relative paths to work from the Contents/Resources directory
	changeToResourcesDirectory();
	
	active_ = true;
	[app finishLaunching];
}


// -- active state

void Application::setActive(bool active) {
	active_ = active;
}


// -- quit state

void Application::quitNow() {
	quit_ = true;
}

void Application::resetShouldQuitFlag() {
	quit_ = false;
}


} // ns stardazed
