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
	// FIXME: either refactor App to be a singleton or something else
//	stardazed::quitNow();
	return NO;
}
@end


@interface SDOpenGLView : NSOpenGLView {}
@end
@implementation SDOpenGLView
@end


namespace render = stardazed::render;


static NSOpenGLPixelFormat* pixelFormatForContextDescriptor(const render::RenderContextDescriptor& desc) {
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
	if (desc.bufferMethod == render::BufferingMethod::DoubleBuffer)
		boolAttr(NSOpenGLPFADoubleBuffer);
	else
		boolAttr(NSOpenGLPFATripleBuffer);

	// FSAA method
	if (desc.fsaa != render::FullscreenAntiAliasMethod::None) {
		if (desc.fsaa == render::FullscreenAntiAliasMethod::SSAA)
			boolAttr(NSOpenGLPFASupersample);
		else
			boolAttr(NSOpenGLPFAMultisample);
		valueAttr(NSOpenGLPFASampleBuffers, 1);
		valueAttr(NSOpenGLPFASamples, desc.antiAliasSamples);
	}

	// default frame buffer colour size
	valueAttr(NSOpenGLPFAColorSize, 24);
	valueAttr(NSOpenGLPFAAlphaSize, 8);
	
	// default depth buffer size
	if (desc.depthBits > 0)
		valueAttr(NSOpenGLPFADepthSize, desc.depthBits);

	attrs.push_back(0);
	return [[NSOpenGLPixelFormat alloc] initWithAttributes: attrs.data()];
}


static SDOpenGLView* createOpenGLView(const NSRect frame, const render::RenderContextDescriptor& desc) {
	auto pixelFormat = pixelFormatForContextDescriptor(desc);
	SDOpenGLView* oglView = [[SDOpenGLView alloc] initWithFrame:frame pixelFormat: pixelFormat];
	
	GLint sync = desc.verticalSync ? 1 : 0;
	[[oglView openGLContext] setValues: &sync forParameter: NSOpenGLCPSwapInterval];

	return oglView;
}


static NSWindow* createRenderWindow(const render::RenderContextDescriptor& desc) {
	NSRect frame;
	NSUInteger styleOptions;
	if (desc.fullscreen) {
		// In fullscreen mode, the width and height are used only for
		// the backbuffer of the OpenGL context. The window is plain.
		frame = [[NSScreen mainScreen] frame];
		styleOptions = NSBorderlessWindowMask;
	}
	else {
		// In windowed mode the window content is sized equal to
		// the viewport and looks like a normal window with titlebar
		// and close control.
		frame = NSMakeRect(0, 0, (CGFloat)desc.width, (CGFloat)desc.height);
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
	if (desc.fullscreen) {
		[window setLevel: NSScreenSaverWindowLevel];
		[window setHidesOnDeactivate: YES];
	}
	
	// Add main content view (a subclass of an NSOpenGLView)
	SDOpenGLView *contentView = createOpenGLView(frame, desc);
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


OpenGLContext::OpenGLContext(const RenderContextDescriptor& descriptor)
: platformData_{ std::make_unique<PlatformData>() }
{
	NSWindow *window = createRenderWindow(descriptor);

	id delegate = [[SDWindowDelegate alloc] init];
	[window setDelegate: delegate];

	[window makeKeyAndOrderFront: nil];

	platformData_->coverWindow = window;
	platformData_->windowDelegate = delegate;
	platformData_->glContext = [[platformData_->coverWindow contentView] openGLContext];
}


OpenGLContext::~OpenGLContext() {
	// need this defined _here_ because of the pimpl idiom using a unique_ptr
	// http://stackoverflow.com/questions/9954518/stdunique-ptr-with-an-incomplete-type-wont-compile
}


Mesh* OpenGLContext::makeStaticMesh(const MeshDescriptor& mesh) {
	return meshPool_.emplace(mesh);
}


Shader* OpenGLContext::loadShaderFromPath(ShaderType type, const std::string& path) {
	return shaderPool_.emplace(type, readTextFile(path));
}


Pipeline* OpenGLContext::makePipeline(const PipelineDescriptor& descriptor) {
	return pipelinePool_.emplace(descriptor);
}


void OpenGLContext::swap() {
	[platformData_->glContext flushBuffer];
}


} // ns render
} // ns stardazed
