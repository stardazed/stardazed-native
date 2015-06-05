// ------------------------------------------------------------------
// render::Shader - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_SHADER_H
#define SD_RENDER_SHADER_H

#include "system/Config.hpp"
#include <vector>

namespace stardazed {
namespace render {


enum class ShaderType {
	Vertex,
	Hull,      // OpenGL term: Tesselation Control
	Domain,    // OpenGL term: Tesselation Evaluation
	Geometry,
	Fragment   // DirectX term: pixel
};


class Shader;


enum class TransformFeedbackAction : uint32 {
	CapturePrimitive,
	NextBuffer,
	SkipComponents
};


struct TransformFeedbackCommand {
	TransformFeedbackAction action;
	union {
		const char* name = nullptr;
		uint32 count;
	};
};


struct TransformFeedbackDescriptor {
	std::vector<TransformFeedbackCommand> varyings;

	void pushCaptureAction(const char* name) {
		TransformFeedbackCommand command {};
		command.action = TransformFeedbackAction::CapturePrimitive;
		command.name = name;
		varyings.push_back(command);
	}

	void pushNextBufferAction() {
		TransformFeedbackCommand command {};
		command.action = TransformFeedbackAction::NextBuffer;
		varyings.push_back(command);
	}

	void pushSkipComponentsAction(int count) {
		TransformFeedbackCommand command {};
		command.action = TransformFeedbackAction::SkipComponents;
		command.count = static_cast<uint32_t>(count);
		varyings.push_back(command);
	}
};


} // ns render
} // ns stardazed

#endif
