// ------------------------------------------------------------------
// Mac OS X Application Scaffolding - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#include <memory>
#include "Application.h"
#include "../include/RenderTarget.h"


namespace stardazed {

	namespace {
		void initAppKit() {
			if (NSApp)
				return;
			
			[NSApplication sharedApplication];
			[NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
			[NSApp finishLaunching];
		}

		std::unique_ptr<RenderTarget> renderTarget;
		bool quit = false;
	}
	
	
	void quitNow() {
		quit = true;
	}


	Application::Application() {
		initAppKit();

		renderTarget = std::make_unique<RenderTarget>(1280, 960, true);
	}
	
	bool Application::shouldQuit() {
		return quit;
	}
	
	void Application::yieldSystem() {
		NSEvent *ev;
		
		do {
			ev = [NSApp nextEventMatchingMask: NSAnyEventMask
			                        untilDate: Nil
						               inMode: NSDefaultRunLoopMode
						              dequeue: YES];
		} while (ev);
		
		renderTarget->swap();
	}

}
