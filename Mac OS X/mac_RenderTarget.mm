// ------------------------------------------------------------------
// mac_RenderTarget - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include <Cocoa/Cocoa.h>
#include <vector>
#include "../include/RenderTarget.h"


@interface SDWindowDelegate : NSObject {}
@end

@implementation SDWindowDelegate
- (BOOL)windowShouldClose:(id)sender {
	return NO;
}
@end


@interface SDOpenGLView : NSOpenGLView {}
@end

@implementation SDOpenGLView
@end


namespace {

	SDOpenGLView* createOpenGLView(const NSRect frame, bool fullscreen) {
		std::vector<NSOpenGLPixelFormatAttribute> attrs = {
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFAAccelerated,
			NSOpenGLPFANoRecovery,
			NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		};
		if (fullscreen) {
			attrs.push_back(NSOpenGLPFAFullScreen);
		}
		else {
			attrs.push_back(NSOpenGLPFAWindow);
		}
		attrs.push_back(0);
		NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: attrs.data()];
		
		SDOpenGLView *oglView = [[SDOpenGLView alloc] initWithFrame:frame pixelFormat: pixelFormat];
		return oglView;
	}


	NSWindow* createRenderWindow(int width, int height, bool fullscreen) {
		NSRect frame;
		NSUInteger styleOptions;
		if (fullscreen) {
			// In fullscreen mode, the width and height are used only for
			// the backbuffer of the OpenGL context. The window is plain.
			frame = [[NSScreen mainScreen] frame];
			styleOptions = NSBorderlessWindowMask;
		}
		else {
			// In windowed mode the window content is sized equal to
			// the viewport and looks like a normal window with titlebar
			// and close control.
			frame = NSMakeRect(0, 0, (CGFloat)width, (CGFloat)height);
			styleOptions = NSTitledWindowMask | NSClosableWindowMask;
		}
		
		NSWindow *window = [[NSWindow alloc]
		    initWithContentRect: frame
		              styleMask: styleOptions
		                backing: NSBackingStoreBuffered
		                  defer: NO
		];
		[window setTitle: @"Stardazed"];
		[window setAcceptsMouseMovedEvents: YES];
		[window setOpaque: YES];
		[window setBackgroundColor: [NSColor blackColor]];
		[window center];
		
		// In fullscreen mode we need to hover above all other windows
		// and be nice and hide ourselves when we're moved to the bg.
		if (fullscreen) {
			[window setLevel: NSMainMenuWindowLevel + 1];
			[window setHidesOnDeactivate: YES];
		}

		id delegate = [[SDWindowDelegate alloc] init];
		[window setDelegate: delegate];
		
		// Add main content view (a subclass of an NSOpenGLView)
		SDOpenGLView *contentView = createOpenGLView(frame, fullscreen);
		[window setContentView: contentView];
		[[contentView openGLContext] makeCurrentContext];
		
		GLint sync = 1;
		[[contentView openGLContext] setValues: &sync forParameter: NSOpenGLCPSwapInterval];

		return window;
	}

}


namespace stardazed {

	class RenderTarget::Impl {
	public:
		NSWindow *coverWindow;
	};

	RenderTarget::RenderTarget(int width, int height, bool fullscreen)
	: width { width }, height { height }, fullscreen { fullscreen }
	{
		pimpl = std::make_unique<Impl>();
		pimpl->coverWindow = createRenderWindow(width, height, fullscreen);
		
		[pimpl->coverWindow makeKeyAndOrderFront: Nil];
	}

	RenderTarget::~RenderTarget() {}
	
	void RenderTarget::swap() {
		[[[pimpl->coverWindow contentView] openGLContext] flushBuffer];
	}

}
