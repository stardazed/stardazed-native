// ------------------------------------------------------------------
// mac_OpenGLContext.mm - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "mac_OpenGLContext.hpp"

#include "util/ConceptTraits.hpp"
#include "system/Application.hpp"
#include "util/TextFile.hpp"

#import <AppKit/AppKit.h>

#include <vector>
#include <cassert>


@interface SDWindow : NSWindow {}
@end
@implementation SDWindow
- (BOOL)canBecomeKeyWindow {
	return YES;
}
- (BOOL)canBecomeMainWindow {
	return YES;
}
@end


@interface SDWindowDelegate : NSObject<NSWindowDelegate> {}
@end

@implementation SDWindowDelegate
- (BOOL)windowShouldClose:(id)sender {
	SD_UNUSED_PARAM(sender)

	stardazed::quitNow();
	return NO;
}
@end


@interface SDOpenGLView : NSOpenGLView {}
@end
@implementation SDOpenGLView
@end



static NSOpenGLPixelFormat* pixelFormatForRenderOptions(const stardazed::render::ContextOptions &options) {
	using namespace stardazed;
	
	std::vector<NSOpenGLPixelFormatAttribute> attrs = {
		NSOpenGLPFAAccelerated,
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
	};

	auto boolAttr = [&attrs](NSOpenGLPixelFormatAttribute name) {
		attrs.push_back(name);
	};
	auto valueAttr = [&attrs](NSOpenGLPixelFormatAttribute name, NSOpenGLPixelFormatAttribute value) {
		attrs.push_back(name);
		attrs.push_back(value);
	};

	// double or triple buffering
	if (options.bufferMethod == render::BufferingMethod::DoubleBuffer)
		boolAttr(NSOpenGLPFADoubleBuffer);
	else
		boolAttr(NSOpenGLPFATripleBuffer);

	// FSAA method
	if (options.fsaa != render::FullscreenAntiAliasMethod::None) {
		if (options.fsaa == render::FullscreenAntiAliasMethod::SSAA)
			boolAttr(NSOpenGLPFASupersample);
		else
			boolAttr(NSOpenGLPFAMultisample);
		valueAttr(NSOpenGLPFASampleBuffers, 1);
		valueAttr(NSOpenGLPFASamples, options.antiAliasSamples);
	}
	
	// buffer depths
	valueAttr(NSOpenGLPFAColorSize, 24);
	valueAttr(NSOpenGLPFAAlphaSize, 8);
	if (options.depthBits > 0)
		valueAttr(NSOpenGLPFADepthSize, options.depthBits);
	if (options.stencilBits > 0)
		valueAttr(NSOpenGLPFAStencilSize, options.stencilBits);
	if (options.accumulatorBits > 0)
		valueAttr(NSOpenGLPFAAccumSize, options.accumulatorBits);
	if (options.auxBufferCount > 0)
		valueAttr(NSOpenGLPFAAuxBuffers, options.auxBufferCount);

	attrs.push_back(0);
	return [[NSOpenGLPixelFormat alloc] initWithAttributes: attrs.data()];
}


static SDOpenGLView* createOpenGLView(const NSRect frame, const stardazed::render::ContextOptions &options) {
	NSOpenGLPixelFormat* pixelFormat = pixelFormatForRenderOptions(options);
	SDOpenGLView *oglView = [[SDOpenGLView alloc] initWithFrame:frame pixelFormat: pixelFormat];
	
	GLint sync = options.verticalSync ? 1 : 0;
	[[oglView openGLContext] setValues: &sync forParameter: NSOpenGLCPSwapInterval];

	return oglView;
}


static NSWindow* createRenderWindow(const stardazed::render::ContextOptions &options) {
	NSRect frame;
	NSUInteger styleOptions;
	if (options.fullscreen) {
		// In fullscreen mode, the width and height are used only for
		// the backbuffer of the OpenGL context. The window is plain.
		frame = [[NSScreen mainScreen] frame];
		styleOptions = NSBorderlessWindowMask;
	}
	else {
		// In windowed mode the window content is sized equal to
		// the viewport and looks like a normal window with titlebar
		// and close control.
		frame = NSMakeRect(0, 0, (CGFloat)options.width, (CGFloat)options.height);
		styleOptions = NSTitledWindowMask | NSClosableWindowMask;
	}
	
	SDWindow *window = [[SDWindow alloc]
		initWithContentRect: frame
				  styleMask: styleOptions
					backing: NSBackingStoreBuffered
					  defer: NO
	];
	[window setTitle: @"Stardazed"];
	[window setAcceptsMouseMovedEvents: YES];
	[window setOpaque: YES];
	[window center];
	
	// In fullscreen mode we need to hover above all other windows
	// and be nice and hide ourselves when we're moved to the bg.
	if (options.fullscreen) {
		[window setLevel: NSScreenSaverWindowLevel];
		[window setHidesOnDeactivate: YES];
	}

	// Add main content view (a subclass of an NSOpenGLView)
	SDOpenGLView *contentView = createOpenGLView(frame, options);
	[window setContentView: contentView];
	[[contentView openGLContext] makeCurrentContext];

	return window;
}


namespace stardazed {
namespace render {

class OpenGLContext::PlatformData {
public:
	NSWindow* coverWindow;
	id windowDelegate;
	NSOpenGLContext* glContext;
};


static void setupGL(const ContextOptions& rco) {
	glViewport(0, 0, rco.width, rco.height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(0, 0, 0, 0);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}


OpenGLContext::OpenGLContext(ContextOptions rco)
: Context<MacOpenGLContextTag>(rco)
, platformData_{ std::make_unique<PlatformData>() }
{
	NSWindow *window = createRenderWindow(options);

	id delegate = [[SDWindowDelegate alloc] init];
	[window setDelegate: delegate];

	[window makeKeyAndOrderFront: nil];

	platformData_->coverWindow = window;
	platformData_->windowDelegate = delegate;
	platformData_->glContext = [[platformData_->coverWindow contentView] openGLContext];

	setupGL(rco);
}


OpenGLContext::~OpenGLContext() {
	// need this defined _here_ because of the pimpl idiom using a unique_ptr
	// http://stackoverflow.com/questions/9954518/stdunique-ptr-with-an-incomplete-type-wont-compile
}


OpenGLMesh OpenGLContext::makeStaticMesh(const Mesh& mesh) {
	return { mesh };
}


OpenGLShader OpenGLContext::loadShaderFromPath(ShaderType type, const std::string& path) {
	return { type, readTextFile(path) };
}


OpenGLPipeline OpenGLContext::makePipeline() {
	return {};
}


void OpenGLContext::swap() {
	[platformData_->glContext flushBuffer];
}


} // ns render
} // ns stardazed
