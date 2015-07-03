// ------------------------------------------------------------------
// mac_GLRenderContext.mm - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/mac_GLRenderContext.hpp"

#include "util/ConceptTraits.hpp"
#include "system/Application.hpp"
#include "util/TextFile.hpp"
#include "system/Logging.hpp"

#import <AppKit/AppKit.h>


//  ____  _       _    __
// |  _ \| | __ _| |_ / _| ___  _ __ _ __ ___
// | |_) | |/ _` | __| |_ / _ \| '__| '_ ` _ \
// |  __/| | (_| | |_|  _| (_) | |  | | | | | |
// |_|   |_|\__,_|\__|_|  \___/|_|  |_| |_| |_|
//

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
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
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

	// drawable colour pixel format (fixed)
	valueAttr(NSOpenGLPFAColorSize, 24);
	valueAttr(NSOpenGLPFAAlphaSize, 8);

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
: shadersBasePath_{ Application::dataPath(), "shaders/" }
, texturesBasePath_{ Application::dataPath(), "textures/" }
, platformData_{ std::make_unique<PlatformData>() }
{
	NSWindow *window = createRenderWindow(descriptor);

	id delegate = [[SDWindowDelegate alloc] init];
	[window setDelegate: delegate];

	[window makeKeyAndOrderFront: nil];

	platformData_->coverWindow = window;
	platformData_->windowDelegate = delegate;
	renderWidth_ = descriptor.width;
	renderHeight_ = descriptor.height;
	
	SDOpenGLView *contentView = [platformData_->coverWindow contentView];
	platformData_->glContext = [contentView openGLContext];

	if (descriptor.fullscreen) {
		platformData_->fullscreenOptions = @{};
		[contentView enterFullScreenMode:[NSScreen mainScreen] withOptions:platformData_->fullscreenOptions];
	}

	// after the window is created and position (including optional fullscreen,)
	// retrieve the real frame size for blitting, etc.
	frameWidth_ = contentView.frame.size.width;
	frameHeight_ = contentView.frame.size.height;
	
	platformData_->verticalSync = descriptor.verticalSync;
	
	// -- reserve some reasonable arrays
	// FIXME: allow for outside specification of these numbers
	pipelinePool_.reserve(128);
	shaderPool_.reserve(128);
	texturePool_.reserve(128);
	meshPool_.reserve(128);
	frameBufferPool_.reserve(16);
	depthStencilTestPool_.reserve(32);

	// -- this is disabled by default and we leave it on for the entire
	// duration of the app.
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


void RenderContext::swap() {
	[platformData_->glContext flushBuffer];
}


//   ___  _                                      
//  / _ \| |__  ___  ___ _ ____   _____ _ __ ___ 
// | | | | '_ \/ __|/ _ \ '__\ \ / / _ \ '__/ __|
// | |_| | |_) \__ \  __/ |   \ V /  __/ |  \__ \
//  \___/|_.__/|___/\___|_|    \_/ \___|_|  |___/
//                                               

bool RenderContext::isFullscreen() const {
	return platformData_->fullscreenOptions != nullptr;
}


bool RenderContext::usesVerticalSync() const {
	return platformData_->verticalSync;
}


//  ____
// |  _ \ ___  ___  ___  _   _ _ __ ___ ___  ___
// | |_) / _ \/ __|/ _ \| | | | '__/ __/ _ \/ __|
// |  _ <  __/\__ \ (_) | |_| | | | (_|  __/\__ \
// |_| \_\___||___/\___/ \__,_|_|  \___\___||___/
//

Shader* RenderContext::loadShaderNamed(const std::string& fileName) {
	fs::Path fileAsPath { fileName };

	// -- determine shader type based on file extension
	auto ext = fileAsPath.extension();
	ShaderType type;
	if (ext == "vert")
		type = ShaderType::Vertex;
	else if (ext == "frag")
		type = ShaderType::Fragment;
	else if (ext == "geom")
		type = ShaderType::Geometry;
	else {
		assert(! "Unknown file extension for shader");
		return nullptr;
	}

	// create, append and return shader instance
	shaderPool_.emplace_back(type, readTextFile(fs::Path{ shadersBasePath_, fileName }));
	return &shaderPool_.back();
}


Pipeline* RenderContext::makePipeline(const PipelineDescriptor& descriptor) {
	pipelinePool_.emplace_back(descriptor);
	return &pipelinePool_.back();
}


Texture* RenderContext::makeTexture(const TextureDescriptor& descriptor) {
	texturePool_.emplace_back(descriptor);
	return &texturePool_.back();
}


// FIXME: the UseMipMaps params here only _restrict_ the use of mipmaps,
// needs to be extended that it will allocate and, if necessary, generate them
// if it is set to Yes regardless of Provider data.

Texture* RenderContext::makeTextureFromProvider(const PixelDataProvider& provider, TextureClass textureClass, UseMipMaps useMipMaps) {
	assert(textureClass != TextureClass::TexCube);

	auto texDesc = makeTexDescFromPixelDataProvider(provider, textureClass);
	if (useMipMaps == UseMipMaps::No)
		texDesc.mipmaps = 1;
	
	auto texture = makeTexture(texDesc);
	texture->writeProviderPixels(provider);
	
	return texture;
}


Texture* RenderContext::makeCubeMapTextureFromProviders(const PixelDataProvider& posX, const PixelDataProvider& negX,
									 const PixelDataProvider& posY, const PixelDataProvider& negY,
									 const PixelDataProvider& posZ, const PixelDataProvider& negZ,
									 UseMipMaps useMipMaps)
{
	auto texDesc = makeTexDescFromPixelDataProvider(posX, TextureClass::TexCube);
	if (useMipMaps == UseMipMaps::No)
		texDesc.mipmaps = 1;

	auto texture = makeTexture(texDesc);
	texture->writeProviderPixels(posX, CubeMapFace::PosX);
	texture->writeProviderPixels(negX, CubeMapFace::NegX);
	texture->writeProviderPixels(posY, CubeMapFace::PosY);
	texture->writeProviderPixels(negY, CubeMapFace::NegY);
	texture->writeProviderPixels(posZ, CubeMapFace::PosZ);
	texture->writeProviderPixels(negZ, CubeMapFace::NegZ);

	return texture;
}


FrameBuffer* RenderContext::makeFrameBuffer(const FrameBufferDescriptor& descriptor) {
	frameBufferPool_.emplace_back(descriptor);
	return &frameBufferPool_.back();
}


FrameBuffer* RenderContext::makeFrameBufferAllocatingTextures(const FrameBufferAllocationDescriptor& fbaDesc) {
	auto descriptor = allocateTexturesForFrameBuffer(fbaDesc);
	return makeFrameBuffer(descriptor);
}


DepthStencilTest* RenderContext::makeDepthStencilTest(const DepthStencilTestDescriptor& dstDesc) {
	depthStencilTestPool_.emplace_back(dstDesc);
	return &depthStencilTestPool_.back();
}


Mesh* RenderContext::makeMesh(const MeshDescriptor& meshDescriptor) {
	meshPool_.emplace_back(meshDescriptor);
	return &meshPool_.back();
}


Mesh* RenderContext::makeMesh(const MeshData& meshData) {
	return makeMesh(meshData.defaultDescriptor());
}


FrameBufferDescriptor RenderContext::allocateTexturesForFrameBuffer(const FrameBufferAllocationDescriptor& desc) {
	FrameBufferDescriptor fbDesc {};
	
	auto width = desc.width;
	auto height = desc.height;
	
	// -- default to viewport size if not explicitly specified
	if (width == 0 && height == 0) {
		width = renderPixelWidth();
		height = renderPixelHeight();
	}
	
	// -- colour
	for (auto colourAttIndex = 0u; colourAttIndex < desc.colourFormats.size(); ++colourAttIndex) {
		if (desc.colourFormats[colourAttIndex] != PixelFormat::None) {
			TextureDescriptor texDesc {};
			texDesc.textureClass = TextureClass::Tex2D;
			texDesc.dim.width = width;
			texDesc.dim.height = height;
			texDesc.samples = desc.samples;
			texDesc.pixelFormat = desc.colourFormats[colourAttIndex];
			texDesc.usageHint = desc.colourUsageHints[colourAttIndex];

			auto& attachment = fbDesc.colourAttachments[colourAttIndex];
			attachment.texture = makeTexture(texDesc);
		}
	}

	// -- depth & stencil
	PixelFormat combinedFormat = PixelFormat::None;
	
	assert(desc.depthFormat == PixelFormat::None ||
		   pixelFormatIsDepthFormat(desc.depthFormat) ||
		   pixelFormatIsDepthStencilFormat(desc.depthFormat));
	assert(desc.stencilFormat == PixelFormat::None ||
		   pixelFormatIsStencilFormat(desc.stencilFormat) ||
		   pixelFormatIsDepthStencilFormat(desc.stencilFormat));

	// -- check if we can use a combined depth/stencil format
	if (pixelFormatIsDepthStencilFormat(desc.depthFormat)) {
		// explicit combined format
		assert(desc.depthFormat == desc.stencilFormat);
		assert(desc.depthUsageHint == desc.stencilUsageHint);
		combinedFormat = desc.depthFormat;
	}
	else {
		// if depth is not a DS format, then stencil cannot be a DS format either
		assert(! pixelFormatIsDepthStencilFormat(desc.stencilFormat));

		// in order to be combined, the usage hints must be the same
		if (desc.depthUsageHint == desc.stencilUsageHint) {
			// check for the available depth/stencil format combinations
			if (desc.stencilFormat == PixelFormat::Stencil8) {
				if (desc.depthFormat == PixelFormat::Depth24I)
					combinedFormat = PixelFormat::Depth24_Stencil8;
				else if (desc.depthFormat == PixelFormat::Depth32F)
					combinedFormat = PixelFormat::Depth32F_Stencil8;
			}
		}
	}

	// -- create the texture(s)
	TextureDescriptor dsTex {};
	dsTex.textureClass = TextureClass::Tex2D;
	dsTex.dim.width = width;
	dsTex.dim.height = height;
	dsTex.samples = desc.samples;

	if (combinedFormat != PixelFormat::None) {
		dsTex.pixelFormat = combinedFormat;
		dsTex.usageHint = desc.depthUsageHint;
		auto depthStencil = makeTexture(dsTex);

		fbDesc.depthAttachment.texture = depthStencil;
		fbDesc.stencilAttachment.texture = depthStencil;
	}
	else {
		if (desc.depthFormat != PixelFormat::None) {
			dsTex.pixelFormat = desc.depthFormat;
			dsTex.usageHint = desc.depthUsageHint;
			fbDesc.depthAttachment.texture = makeTexture(dsTex);
		}
		if (desc.stencilFormat != PixelFormat::None) {
			dsTex.pixelFormat = desc.stencilFormat;
			dsTex.usageHint = desc.stencilUsageHint;
			fbDesc.stencilAttachment.texture = makeTexture(dsTex);
		}
	}
	
	return fbDesc;
}


} // ns render
} // ns stardazed
