// ------------------------------------------------------------------
// MacApplication - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "plat_macosx/mac_Application.hpp"
#include "util/ConceptTraits.hpp"
#include "system/Logging.hpp"

#import <AppKit/AppKit.h>


@interface SDAppDelegate : NSObject<NSApplicationDelegate> {
	stardazed::MacApplication* app_;
}
- (id)initWithApplication:(stardazed::MacApplication *)application;
@property (nonatomic, strong) NSArray* xibObjects;
@end

@implementation SDAppDelegate
- (id)initWithApplication:(stardazed::MacApplication *)application {
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


MacApplication::MacApplication() {
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
	
	inputCtx_ = std::make_unique<input::MacInputContext>();
	
	[app finishLaunching];
}


input::InputContext& MacApplication::input() {
	return *inputCtx_;
}


void MacApplication::changeToResourcesDirectory() {
	const char *resourcePath = [[[NSBundle mainBundle] resourcePath] UTF8String];
	chdir(resourcePath);
}


void MacApplication::quitNow() {
	quit_ = true;
}

bool MacApplication::shouldQuit() {
	return quit_;
}

void MacApplication::resetShouldQuitFlag() {
	quit_ = false;
}


} // ns stardazed
