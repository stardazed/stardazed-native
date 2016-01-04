// ------------------------------------------------------------------
// RunLoop - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNLOOP_H
#define SD_RUNLOOP_H

#include "system/Config.hpp"
#include "system/Time.hpp"
#include "render/RenderContext.hpp"

namespace stardazed {


enum class RunLoopState {
	Idle,
	Running
};


struct SceneController {
	virtual ~SceneController() {}

	virtual void renderFrame(Time) = 0;
	virtual void simulationStep(Time) = 0;

	virtual void resume() {}
	virtual void suspend() {}
	
	virtual void focus() {}
	virtual void blur() {}
};


class RunLoop {
	SceneController* controller_ = nullptr;
	render::RenderContext* renderCtx_ = nullptr;
	
	Time maxFrameTime_ = time::hertz(4);
	Time minFrameTime_ = time::hertz(120); // only relevant in non-vsync context
	Time lastFrameTime_ = time::zero();
	Time globalTime_ = time::zero();
	
	RunLoopState runState_ = RunLoopState::Idle;
	
public:
	RunLoop();
	
	SceneController* sceneController() { return controller_; }
	void setSceneController(SceneController&);
	
	void setRenderContext(render::RenderContext&);
	
	void mainLoop();
	
	// start/stop is called by Application when the app
	// becomes or ceases to be the frontmost app in the system, resp.
	void start();
	void stop();
};



RunLoop& defaultRunLoop();


} // ns stardazed

#endif
