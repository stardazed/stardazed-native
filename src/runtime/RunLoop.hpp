// ------------------------------------------------------------------
// runtime::RunLoop - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNTIME_RUNLOOP_H
#define SD_RUNTIME_RUNLOOP_H

#include "system/Config.hpp"
#include "system/Application.hpp"
#include "runtime/Client.hpp"
#include "system/Time.hpp"
#include "scene/SceneController.hpp"

namespace stardazed {
namespace runtime {


class FPSStats {
	time::Duration t0_, lastT_;
	int frames_ = -1;
	
public:
	void frameRendered() {
		++frames_;

		auto t = time::now();
		if (frames_ == 0)
			t0_ = t;
		lastT_ = t;
	}
	
	float fps() const {
		if (frames_ > 60)
			return frames_ / (lastT_ - t0_).count();
		return 0;
	}
};
	

class RunLoop {
	Client& client_;
	scene::SceneController* sceneCtl_;

	time::Duration simulationFrameTime_, renderFrameTime_;
	time::Duration simulationLag_ = time::zero(), renderLag_ = time::zero();
	time::Duration previousTime_;
	
	void suspend();
	void resume();
	void mainLoop();

public:
	FPSStats stats;

	RunLoop(Client& client);
	
	void setSimulationFPS(int sfps) { simulationFrameTime_ = time::hertz(sfps); }
	void setMaxRenderFPS(int rfps) { renderFrameTime_ = time::hertz(rfps); }
	
	void setSceneController(scene::SceneController&);
	
	void run();
};


} // ns runtime
} // ns stardazed

#endif
