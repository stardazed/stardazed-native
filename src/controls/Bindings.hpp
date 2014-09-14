// ------------------------------------------------------------------
// controls::Bindings - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTROLS_BINDINGS_H
#define SD_CONTROLS_BINDINGS_H

#include "system/Config.hpp"
#include "device/DevicesContext.hpp"

#include <memory>

namespace stardazed {
namespace controls {


class Binding {
public:
	virtual ~Binding() {}
	virtual float value(const DevicesSnapshot& dss) const = 0;
};


class KeyPair : public Binding {
	device::Key keyA_, keyB_;
	
public:
	constexpr KeyPair(device::Key a, device::Key b)
	: keyA_(a)
	, keyB_(b)
	{}
	
	float value(const DevicesSnapshot& dss) const override {
		float a = dss.keyboard().isPressed(keyA_) ? -1 : 0,
		b = dss.keyboard().isPressed(keyB_) ?  1 : 0;
		
		return a + b;
	}
};


class Alternate : public Binding {
	std::unique_ptr<Binding> primary_, secondary_;
public:
	Alternate(const Binding& primary, const Binding& secondary)
	: primary_(std::make_unique(primary))
	, secondary_(std::make_unique(secondary))
	{}
	
	float value(const DevicesSnapshot& dss) const override {
		float val = primary->value(dss);
		if (val == 0)
			return secondary->value(dss);
		return val;
	}
};



class CubeController {
	Binding* axisX_;
	Vec2 impulse_ = {0,0};

public:
	CubeController() {
		axisX_ = Alternate(
			KeyPair{device::Key::A, device::Key::D},
			KeyPair{device::Key::LeftArrow, device::Key::RightArrow}
		);
	}

	void handleControls(const device::DevicesSnapshot& dss) {
		impulse_ = { axisX_.value(), axisZ_.value() };
	}

};


} // ns controls
} // ns stardazed

#endif
