// ------------------------------------------------------------------
// mac_Application - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/mac_Application.hpp"
#include "system/Time.hpp"
#include "system/Logging.hpp"
#include "util/ConceptTraits.hpp"
#include "runtime/Game.hpp"

#include <thread>

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
runtime::Game* Application::game_ = nullptr;


void Application::setActive(bool active) {
	active_ = active;
	if (game_) {
		if (active_)
			game_->resume();
		else
			game_->suspend();
	}
}


static void changeToResourcesDirectory() {
	const char *resourcePath = [[[NSBundle mainBundle] resourcePath] UTF8String];
	chdir(resourcePath);
}


const fs::Path& Application::dataPath() {
	static fs::Path dataPath_s { "data" };
	return dataPath_s;
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


} // ns stardazed
