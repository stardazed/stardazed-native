// ------------------------------------------------------------------
// mac_Application - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/mac_Application.hpp"
#include "system/Logging.hpp"

#import <AppKit/AppKit.h>


@interface SDAppDelegate : NSObject<NSApplicationDelegate> {
	stardazed::Application* app_;
}
- (id)initWithApplication:(stardazed::Application *)application;
@property (nonatomic, strong) NSArray* xibObjects;
@end

@implementation SDAppDelegate
- (id)initWithApplication:(stardazed::Application *)application {
	app_ = application;
	return self;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
	SD_UNUSED_PARAM(sender)
	// since we like control and to keep things sort of similar to non-OS X env
	// we return cancel terminate, but set the global quit flag to true so
	// our own main loop can exit gracefully
	app_->quitNow();
	return NSTerminateCancel;
}
@end


namespace stardazed {


Application::Application() {
	auto app = [NSApplication sharedApplication];
	[app setActivationPolicy: NSApplicationActivationPolicyRegular];
	
	auto appDelegate = [[SDAppDelegate alloc] initWithApplication:this];
	[app setDelegate: appDelegate];

	// -- install menubar, etc.
	NSArray *tlo;
	[[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:appDelegate topLevelObjects: &tlo];
	appDelegate.xibObjects = tlo;

	// -- allow relative paths to work from the Contents/Resources directory
	changeToResourcesDirectory();
	
	[app finishLaunching];
}


void Application::changeToResourcesDirectory() {
	const char *resourcePath = [[[NSBundle mainBundle] resourcePath] UTF8String];
	chdir(resourcePath);
}


void Application::quitNow() {
	quit_ = true;
}

bool Application::shouldQuit() {
	return quit_;
}

void Application::resetShouldQuitFlag() {
	quit_ = false;
}


} // ns stardazed
