// ------------------------------------------------------------------
// mac_GLRenderContext.mm - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/mac_GLRenderContext.hpp"

#include "util/ConceptTraits.hpp"
#include "system/Application.hpp"
#include "util/TextFile.hpp"

#import <AppKit/AppKit.h>

#include <vector>


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
	sd::Application::quitNow();
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

	// frame buffer colour size (fixed)
	valueAttr(NSOpenGLPFAColorSize, 24);
	valueAttr(NSOpenGLPFAAlphaSize, 8);
	
	// depth and stencil buffer size
	if (desc.depthBits != 0) {
		assert(desc.depthBits == 16 || desc.depthBits == 24 || desc.depthBits == 32);
		valueAttr(NSOpenGLPFADepthSize, desc.depthBits);
	}
	if (desc.stencilBits != 0) {
		assert(desc.stencilBits == 8);
		valueAttr(NSOpenGLPFAStencilSize, desc.stencilBits);
	}

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
	NSRect frame = NSMakeRect(0, 0, (CGFloat)desc.width, (CGFloat)desc.height);
	NSUInteger styleOptions = NSTitledWindowMask | NSClosableWindowMask;
	
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


class RenderContext::PlatformData {
public:
	NSWindow* coverWindow;
	id windowDelegate;
	NSOpenGLContext* glContext;
	NSDictionary* fullscreenOptions = nullptr;
	bool verticalSync;
	uint32 width, height;
};


RenderContext::RenderContext(const RenderContextDescriptor& descriptor)
: platformData_{ std::make_unique<PlatformData>() }
{
	NSWindow *window = createRenderWindow(descriptor);

	id delegate = [[SDWindowDelegate alloc] init];
	[window setDelegate: delegate];

	[window makeKeyAndOrderFront: nil];

	platformData_->coverWindow = window;
	platformData_->windowDelegate = delegate;
	width_ = descriptor.width;
	height_ = descriptor.height;
	
	SDOpenGLView *contentView = [platformData_->coverWindow contentView];
	platformData_->glContext = [contentView openGLContext];

	if (descriptor.fullscreen) {
		platformData_->fullscreenOptions = @{};
		[contentView enterFullScreenMode:[NSScreen mainScreen] withOptions:platformData_->fullscreenOptions];
	}
	
	platformData_->verticalSync = descriptor.verticalSync;
	
	// -- reserve some reasonable arrays
	// FIXME: allow for outside specification of these numbers
	pipelinePool_.reserve(128);
	shaderPool_.reserve(128);
	programPool_.reserve(128);

	// -- some sensible global defaults
	// FIXME: these may/should likely go somewhere else?
	glClearColor(0, 0, 0, 0);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}


RenderContext::~RenderContext() {
	// need this defined _here_ because of the pimpl idiom using a unique_ptr
	// http://stackoverflow.com/questions/9954518/stdunique-ptr-with-an-incomplete-type-wont-compile
	
	if (platformData_->fullscreenOptions != nullptr) {
		SDOpenGLView *contentView = [platformData_->coverWindow contentView];
		[contentView exitFullScreenModeWithOptions: platformData_->fullscreenOptions];
	}
}


Shader* RenderContext::loadShaderFromPath(ShaderType type, const std::string& path) {
	shaderPool_.emplace_back(type);
	auto shader = &shaderPool_.back();
	shader->compileSource(readTextFile(path));
	return shader;
}


Program* RenderContext::makeShaderProgram(Shader& shader) {
	programPool_.emplace_back();
	auto shaderProgram = &programPool_.back();
	shaderProgram->setSeparable();
	shaderProgram->attach(shader);
	shaderProgram->link();
	return shaderProgram;
}


Pipeline* RenderContext::makePipeline(const PipelineDescriptor& descriptor) {
	pipelinePool_.emplace_back(descriptor);
	return &pipelinePool_.back();
}


Pipeline* RenderContext::makePipeline(const SSOPipelineDescriptor& descriptor) {
	pipelinePool_.emplace_back(descriptor);
	return &pipelinePool_.back();
}


void RenderContext::swap() {
	[platformData_->glContext flushBuffer];
}


bool RenderContext::isFullscreen() const {
	return platformData_->fullscreenOptions != nullptr;
}


bool RenderContext::usesVerticalSync() const {
	return platformData_->verticalSync;
}


} // ns render
} // ns stardazed
