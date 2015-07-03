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
	// this is a direct mirror of the scalar/vector data in StandardMaterial
	// the layouts of these structures must be exactly the same
	math::Vec3 mainColour;
	float pad__;
	math::Vec3 specularColour;
	float specularExponent;
	math::Vec2 textureScale;
	math::Vec2 textureOffset;
};


StandardModel::StandardModel(const StandardModelDescriptor& desc, StandardShader& shader)
: descriptor_(desc)
, mesh_(*desc.mesh)
, constMatBuffer_{ BufferRole::ConstantBuffer, BufferUpdateFrequency::Never, BufferClientAccess::WriteOnly }
, shader_(shader)
{
	auto cma = std::make_unique<ConstStandardMaterial[]>(32); // mirrors MAX_MATERIALS in StandardShader.frag
	size32 cmaSize = size32_cast(32 * sizeof(ConstStandardMaterial));
	
	std::for_each(begin(desc.materials), end(desc.materials),
		[cmaPtr = cma.get()](const StandardMaterial& mat) mutable {
			std::memcpy(cmaPtr, &mat, sizeof(ConstStandardMaterial));
			++cmaPtr;
		});

	constMatBuffer_.allocate(cmaSize, cma.get());
}


void StandardModel::render(RenderPass& renderPass, const scene::ProjectionSetup& proj, const scene::Entity& entity) const {
	renderPass.setPipeline(shader_.pipeline());
	renderPass.setMesh(mesh_);
	renderPass.setUniformBuffer(constMatBuffer_, 0);

	shader_.setMatrices(proj.projMat, proj.viewMat, entity.transform.toMatrix4());
	shader_.setLights(math::Vec3{ -0.4, 1, 0.4 });

	for (const FaceGroup& fg : descriptor_.faceGroups) {
		auto& material = descriptor_.materials[fg.materialIx];
		shader_.setMaterial(fg.materialIx, material);

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


void StandardShader::setMaterial(uint32 index, const StandardMaterial& material) {
	auto frag = pipeline_->fragmentShader();

	frag->setUniform(fsMatIndex, index);
//	frag->setTexture(material.albedoMap, 0, fsAlbedoMap);
//	frag->setTexture(material.normalMap, 1, fsNormalMap);
}


} // ns model
} // ns stardazed
