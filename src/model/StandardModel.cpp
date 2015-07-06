// ------------------------------------------------------------------
// model::StandardModel.cpp
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "model/StandardModel.hpp"
#include "render/opengl/Mesh.hpp"
#include "render/opengl/Buffer.hpp"
#include "render/opengl/Pipeline.hpp"

namespace stardazed {
namespace model {


using namespace render;


struct ConstStandardMaterial {
	// this is a direct mirror of the non-sampler data in StandardMaterialDescriptor
	// the layouts of these structures must be exactly the same
	math::Vec3 mainColour;
	float pad__;
	math::Vec3 specularColour;
	float specularExponent;
	math::Vec2 textureScale;
	math::Vec2 textureOffset;
};


StandardMaterial::StandardMaterial()
: materialsConstBuffer_{ BufferRole::ConstantBuffer, BufferUpdateFrequency::Never, BufferClientAccess::ReadWrite }
{
	// determine maximum number of components that can be safely used in both the vertex and fragment shaders
	GLint maxVert, maxFrag, maxConstBlockSize;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVert);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFrag);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxConstBlockSize);

	GLint maxUsableComponents = math::min(maxVert, maxFrag);
	GLint maxBlockSizeLimitedMaterials = maxConstBlockSize / sizeof(ConstStandardMaterial);

	// -- WARNING: this does not take into account any <float sized components in the material
	GLint componentsPerMat = sizeof(ConstStandardMaterial) / sizeof(float);
	auto maxComponentLimitedMaterials = maxUsableComponents / componentsPerMat;

	maxMaterialsPerMappedRange_ = static_cast<uint32>(math::min(maxBlockSizeLimitedMaterials, maxComponentLimitedMaterials));
	rangeBlockSizeBytes_ = maxMaterialsPerMappedRange_ * sizeof(ConstStandardMaterial);
	
	materialsConstBuffer_.allocate(rangeBlockSizeBytes_);
	nextIndex_ = 1; // Indexes are 1-based to allow 0 being a nullptr-like
	maxIndex_ = maxMaterialsPerMappedRange_ - 1;
}


StandardMaterial::Index StandardMaterial::alloc(const StandardMaterialDescriptor& matDesc) {
	assert(nextIndex_ <= maxIndex_);

	size32 sizeBytes = sizeof32<ConstStandardMaterial>();
	size32 offset = nextIndex_ * sizeBytes;
	materialsConstBuffer_.write(sizeBytes, &matDesc, offset);
	
	return { nextIndex_++ };
}


void StandardMaterial::allocMultiple(const StandardMaterialDescriptor* base, uint32 count, StandardMaterial::Index* outIndexesBase) {
	assert(nextIndex_ + count <= maxIndex_);

	size32 structSizeBytes = sizeof32<ConstStandardMaterial>();
	size32 arraySizeBytes =  structSizeBytes * count;
	size32 offsetBytes = nextIndex_ * structSizeBytes;

	// -- copy-append const part of each descriptor into temp array
	auto cma = std::make_unique<ConstStandardMaterial[]>(count);
	
	std::for_each(base, base + count,
		[cmaPtr = cma.get()](const StandardMaterialDescriptor& mat) mutable {
			std::memcpy(cmaPtr, &mat, sizeof(ConstStandardMaterial));
			++cmaPtr;
		});

	// -- copy const materials to GL buffer
	materialsConstBuffer_.write(arraySizeBytes, cma.get(), offsetBytes);

	// -- return optional Indexes
	if (outIndexesBase) {
		auto uptoIndex = nextIndex_ + count;
		for (auto curIndex = nextIndex_; curIndex < uptoIndex; ++curIndex)
			*(outIndexesBase++) = { curIndex };
	}

	nextIndex_ += count;
}


void StandardMaterial::mapMaterialAtBindPoint(Index material, uint32 bindPoint) {
	auto blockIndex = material.index / maxMaterialsPerMappedRange_;
	auto offset = blockIndex * rangeBlockSizeBytes_;
	IndexedUniformBlocks::bindBufferRangeToBindPoint(materialsConstBuffer_, offset, rangeBlockSizeBytes_, bindPoint);
}



StandardModel::StandardModel(const StandardModelDescriptor& desc, StandardShader& shader)
: descriptor_(desc)
, mesh_(*desc.mesh)
, shader_(shader)
{
	materialIndexes_.resize(desc.materials.size());
	standardMaterial().allocMultiple(desc.materials.data(), size32_cast(desc.materials.size()), materialIndexes_.data());
}


StandardMaterial& StandardModel::standardMaterial() {
	static StandardMaterial sm_s;
	return sm_s;
}


void StandardModel::render(RenderPass& renderPass, const scene::ProjectionSetup& proj, const scene::Entity& entity) const {
	renderPass.setPipeline(shader_.pipeline());
	renderPass.setMesh(mesh_);
	
	// TODO: add some material-range thing here
	standardMaterial().mapMaterialAtBindPoint(materialIndexes_[0], 0);

	shader_.setMatrices(proj.projMat, proj.viewMat, entity.transform.toMatrix4());
	shader_.setLights(math::Vec3{ -0.4, 1, 0.4 });

	for (const FaceGroup& fg : descriptor_.faceGroups) {
		auto& material = descriptor_.materials[fg.materialIx];
		shader_.setMaterial(materialIndexes_[fg.materialIx], material);

		uint32 startIndex = fg.fromFaceIx * 3;
		uint32 indexCount = (fg.toFaceIx - fg.fromFaceIx) * 3;
		renderPass.drawIndexedPrimitives(startIndex, indexCount);
	}
}



StandardShader::StandardShader(RenderContext& renderCtx) {
	auto vert = renderCtx.loadShaderNamed("StandardShader.vert");
	auto frag = renderCtx.loadShaderNamed("StandardShader.frag");

	// forward rendering version
	PipelineDescriptor pld;
	pld.colourAttachments[0].pixelFormat = PixelFormat::RGBA8;
	pld.depthPixelFormat = PixelFormat::Depth32F;
	pld.vertexShader = vert;
	pld.fragmentShader = frag;
	pipeline_ = renderCtx.makePipeline(pld);
	
	vsMV = vert->uniformPositionForName("modelViewMatrix");
	vsMVP = vert->uniformPositionForName("modelViewProjectionMatrix");
	vsNM = vert->uniformPositionForName("normalMatrix");
	
	fsNM = frag->uniformPositionForName("normalMatrix");
	
	fsLightDir = frag->uniformPositionForName("lightDirection");
	
	fsMatIndex = frag->uniformPositionForName("materialIndex");
//	fsAlbedoMap = frag->uniformPositionForName("albedoSampler");
//	fsNormalMap = frag->uniformPositionForName("normalSampler");
	
	frag->bindNamedUniformBlockToBindPoint("Materials", 0);
}


void StandardShader::setMatrices(const math::Mat4& projection, const math::Mat4& view, const math::Mat4& model) {
	auto modelViewMat = view * model;
	auto mvpMat = projection * modelViewMat;
	auto normalMat = math::transpose(math::inverse(math::extractSubMatrix<3, 3>(modelViewMat)));

	auto vert = pipeline_->vertexShader();
	auto frag = pipeline_->fragmentShader();

	vert->setUniform(vsMV, modelViewMat);
	vert->setUniform(vsMVP, mvpMat);
	vert->setUniform(vsNM, normalMat);
	
	frag->setUniform(fsNM, normalMat);
}


void StandardShader::setLights(const math::Vec3 dirLight) {
	auto frag = pipeline_->fragmentShader();

	frag->setUniform(fsLightDir, dirLight);
}


void StandardShader::setMaterial(StandardMaterial::Index matIndex, const StandardMaterialDescriptor& material) {
	auto frag = pipeline_->fragmentShader();

	frag->setUniform(fsMatIndex, matIndex.index);
//	frag->setTexture(material.albedoMap, 0, fsAlbedoMap);
//	frag->setTexture(material.normalMap, 1, fsNormalMap);
}


} // ns model
} // ns stardazed
