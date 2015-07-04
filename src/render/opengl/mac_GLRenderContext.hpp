// ------------------------------------------------------------------
// render::mac_GLRenderContext - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MAC_GLRENDERCONTEXT_H
#define SD_RENDER_MAC_GLRENDERCONTEXT_H

#include "system/Config.hpp"
#include "filesystem/FileSystem.hpp"
#include "render/common/RenderContext.hpp"
#include "render/opengl/Shader.hpp"
#include "render/opengl/Pipeline.hpp"
#include "render/opengl/Texture.hpp"
#include "render/opengl/Mesh.hpp"
#include "render/opengl/FrameBuffer.hpp"
#include "render/opengl/RenderPass.hpp"

#include <vector>
#include <string>
#include <memory>

namespace stardazed {
namespace render {


class RenderContext {
public:
	RenderContext(const RenderContextDescriptor&);
	~RenderContext();
	
	Shader* loadShaderNamed(const std::string& fileName);

	Pipeline* makePipeline(const PipelineDescriptor&);

	Texture* makeTexture(const TextureDescriptor&);
	Texture* makeTextureFromProvider(const PixelDataProvider&, TextureClass, UseMipMaps);
	Texture* makeCubeMapTextureFromProviders(const PixelDataProvider& posX, const PixelDataProvider& negX,
											 const PixelDataProvider& posY, const PixelDataProvider& negY,
											 const PixelDataProvider& posZ, const PixelDataProvider& negZ,
											 UseMipMaps useMipMaps);

	Sampler* makeSampler(const SamplerDescriptor&);

	FrameBuffer* makeFrameBuffer(const FrameBufferDescriptor&);
	FrameBuffer* makeFrameBufferAllocatingTextures(const FrameBufferAllocationDescriptor&);
	
	DepthStencilTest* makeDepthStencilTest(const DepthStencilTestDescriptor&);
	
	Mesh* makeMesh(const MeshDescriptor&);
	Mesh* makeMesh(const MeshData&);
	
	void swap();
	
	bool isFullscreen() const;
	bool usesVerticalSync() const;
	
	uint32 renderPixelWidth() const { return renderWidth_; }
	uint32 renderPixelHeight() const { return renderHeight_; }

	uint32 framePixelWidth() const { return frameWidth_; }
	uint32 framePixelHeight() const { return frameHeight_; }

private:
	uint32 renderWidth_, renderHeight_;
	uint32 frameWidth_, frameHeight_;

	// object storage
	std::vector<Pipeline> pipelinePool_;
	std::vector<Shader> shaderPool_;
	std::vector<Texture> texturePool_;
	std::vector<Sampler> samplerPool_;
	std::vector<Mesh> meshPool_;
	std::vector<FrameBuffer> frameBufferPool_;
	std::vector<DepthStencilTest> depthStencilTestPool_;
	
	// resource base paths
	fs::Path shadersBasePath_;
	fs::Path texturesBasePath_;

	// internal methods
	FrameBufferDescriptor allocateTexturesForFrameBuffer(const FrameBufferAllocationDescriptor&);

	// place obj-c stuff in implementation file only
	class PlatformData;
 	std::unique_ptr<PlatformData> platformData_;
};


} // ns render
} // ns stardazed

#endif
