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


//  ___ _                _             _ __  __      _           _      _
// / __| |_ __ _ _ _  __| |__ _ _ _ __| |  \/  |__ _| |_ ___ _ _(_)__ _| |
// \__ \  _/ _` | ' \/ _` / _` | '_/ _` | |\/| / _` |  _/ -_) '_| / _` | |
// |___/\__\__,_|_||_\__,_\__,_|_| \__,_|_|  |_\__,_|\__\___|_| |_\__,_|_|
//

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


StandardMaterialBuffer::StandardMaterialBuffer()
: materialsConstBuffer_{ BufferRole::ConstantBuffer, BufferUpdateFrequency::Never, BufferClientAccess::WriteOnly }
{
	materialsPerBlock_ = ConstantBufferLimits::maximumAccessibleArrayElementsPerBlock<ConstStandardMaterial>();
	rangeBlockSizeBytes_ = materialsPerBlock_ * sizeof32<ConstStandardMaterial>();

	uniformOffsetAlignment_ = ConstantBufferLimits::offsetAlignment();
	rangeBlockSizeBytesAligned_ = math::alignUp(rangeBlockSizeBytes_, uniformOffsetAlignment_);
	
	nextIndex_ = 1; // Indexes are 1-based to allow 0 being a nullptr-like
	maxIndex_ = materialsPerBlock_ - 1;

	auto numBlocks = static_cast<uint32>(std::ceilf((float)maxIndex_ / (float)materialsPerBlock_));
	materialsConstBuffer_.allocate(numBlocks * rangeBlockSizeBytesAligned_);
}


auto StandardMaterialBuffer::append(const StandardMaterialDescriptor& matDesc) -> Index {
	assert(nextIndex_ <= maxIndex_);

	size32 sizeBytes = sizeof32<ConstStandardMaterial>();
	size32 offset = nextIndex_ * sizeBytes;
	materialsConstBuffer_.write(sizeBytes, &matDesc, offset);
	
	return { nextIndex_++ };
}


void StandardMaterialBuffer::appendMultiple(const StandardMaterialDescriptor* base, uint32 count, Index* outIndexesBase) {
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


void StandardMaterialBuffer::mapMaterialAtBindPoint(Index material, uint32 bindPoint) {
	auto blockIndex = material.index / materialsPerBlock_;
	auto offset = blockIndex * rangeBlockSizeBytesAligned_;
	firstBoundMaterialIndex_ = blockIndex * materialsPerBlock_;
	
	// align the offset down to the nearest previous alignment boundary
	offset = math::alignDown(offset, uniformOffsetAlignment_);
	IndexedUniformBlocks::bindBufferRangeToBindPoint(materialsConstBuffer_, offset, rangeBlockSizeBytes_, bindPoint);
}


//  ___ _                _             _ ___ _            _
// / __| |_ __ _ _ _  __| |__ _ _ _ __| / __| |_  __ _ __| |___ _ _
// \__ \  _/ _` | ' \/ _` / _` | '_/ _` \__ \ ' \/ _` / _` / -_) '_|
// |___/\__\__,_|_||_\__,_\__,_|_| \__,_|___/_||_\__,_\__,_\___|_|
//

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


void StandardShader::setMaterial(StandardMaterialBuffer::Index matIndex /*, const StandardMaterialDescriptor& material */) {
	auto frag = pipeline_->fragmentShader();

	frag->setUniform(fsMatIndex, matIndex.index);
//	frag->setTexture(material.albedoMap, 0, fsAlbedoMap);
//	frag->setTexture(material.normalMap, 1, fsNormalMap);
}


//  ___ _                _             _ __  __         _     _ __  __
// / __| |_ __ _ _ _  __| |__ _ _ _ __| |  \/  |___  __| |___| |  \/  |__ _ _ _
// \__ \  _/ _` | ' \/ _` / _` | '_/ _` | |\/| / _ \/ _` / -_) | |\/| / _` | '_|
// |___/\__\__,_|_||_\__,_\__,_|_| \__,_|_|  |_\___/\__,_\___|_|_|  |_\__, |_|
//                                                                    |___/

StandardModelManager::StandardModelManager(RenderContext& renderCtx, scene::TransformComponent& transformComp)
: transformComp_(transformComp)
, stdShader_{renderCtx}
, stdMaterialBuffer_{}
, materialIndexes_{ memory::SystemAllocator::sharedInstance(), 4096 }
, faceGroups_{ memory::SystemAllocator::sharedInstance(), 4096 }
, instanceData_{ memory::SystemAllocator::sharedInstance(), 2048 }
{}


auto StandardModelManager::create(const StandardModelDescriptor& desc) -> Instance {
	auto materialCount = desc.materials.count();
	uint matIndexIndex = materialIndexes_.count();
	auto matIndexPtr = materialIndexes_.prepareForBlockCopy(materialCount);
	stdMaterialBuffer_.appendMultiple(desc.materials.elementsBasePtr(), materialCount, matIndexPtr);
	
	auto faceGroupCount = desc.faceGroups.count();
	uint faceGroupIndex = faceGroups_.count();
	faceGroups_.appendBlock(desc.faceGroups.elementsBasePtr(), faceGroupCount);
	
	instanceData_.extend();
	uint instanceIndex = instanceData_.count() - 1;
	*(instanceData_.elementsBasePtr<0>() + instanceIndex) = desc.mesh;
	*(instanceData_.elementsBasePtr<1>() + instanceIndex) = { matIndexIndex, materialCount };
	*(instanceData_.elementsBasePtr<2>() + instanceIndex) = { faceGroupIndex, faceGroupCount };
	
	return { instanceIndex };
}


void StandardModelManager::linkEntityToModel(scene::Entity entity, Instance instance) {
	entityMap_.insert(entity, { instance, transformComp_.forEntity(entity) });
}


void StandardModelManager::render(RenderPass& renderPass, const scene::ProjectionSetup& proj, scene::Entity entity) {
	// get instance data
	auto modelTrans = entityMap_.find(entity);
	assert(modelTrans);
	auto instance = modelTrans->instance;
	auto modelMatrix = transformComp_.modelMatrix(modelTrans->transformInstance);
	
	auto mesh = *(instanceData_.elementsBasePtr<0>() + instance.ref);
	auto matIndexRange = *(instanceData_.elementsBasePtr<1>() + instance.ref);
	auto faceGroupIndexRange = *(instanceData_.elementsBasePtr<2>() + instance.ref);

	renderPass.setPipeline(stdShader_.pipeline());
	renderPass.setMesh(*mesh);
	
	// TODO: add some material-range thing here
	stdMaterialBuffer_.mapMaterialAtBindPoint(materialIndexes_[matIndexRange.first], 0);
	auto firstBoundMatIndex = stdMaterialBuffer_.firstBoundMaterialIndex();

	stdShader_.setMatrices(proj.projMat, proj.viewMat, modelMatrix);
	stdShader_.setLights(math::Vec3{ -0.4, 1, 0.4 });

	for (auto fgIx = faceGroupIndexRange.first, uptoIx = fgIx + faceGroupIndexRange.count; fgIx < uptoIx; ++fgIx) {
		auto& fg = faceGroups_[fgIx];
		auto matIndex = materialIndexes_[fg.materialIx];
		matIndex.index -= firstBoundMatIndex;
		stdShader_.setMaterial(matIndex);

		uint32 startIndex = fg.fromFaceIx * 3;
		uint32 indexCount = fg.faceCount * 3;
		renderPass.drawIndexedPrimitives(startIndex, indexCount);
	}
}


} // ns model
} // ns stardazed
