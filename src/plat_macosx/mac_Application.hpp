// ------------------------------------------------------------------
// MacApplication - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MACAPPLICATION_H
#define SD_MACAPPLICATION_H

#include "system/Application.hpp"
#include "plat_macosx/mac_InputContext.hpp"

#include <memory>

namespace stardazed {


class MacApplication : public Application {
	std::unique_ptr<input::MacInputContext> inputCtx_;
	bool quit_ = false;
	
	void changeToResourcesDirectory();

public:
	MacApplication();
	
	// -- InputContext access
	input::InputContext& input() override;
	
	// -- handling of Quit signal from system or user
	void quitNow() override;
	bool shouldQuit() override;
	void resetShouldQuitFlag() override;
};


} // ns stardazed

#endif
