// ------------------------------------------------------------------
// io::input - stardazed
// (c) 2016 by Arthur Langereis
// ------------------------------------------------------------------

#include "io/input.hpp"

namespace stardazed {
namespace io {
	

Keyboard::Keyboard()
: keyData_(memory::SystemAllocator::sharedInstance(), 128)
{
	downBase_ = keyData_.elementsBasePtr<0>();
	halfTransBase_ = keyData_.elementsBasePtr<1>();
}


ButtonState Keyboard::keyState(Key kc) const {
	auto index = static_cast<int>(kc);
	return {
		downBase_[index],
		halfTransBase_[index]
	};
}


bool Keyboard::down(Key kc) const {
	auto index = static_cast<int>(kc);
	return downBase_[index];
}


bool Keyboard::pressed(Key kc) const {
	auto index = static_cast<int>(kc);
	return downBase_[index] && (halfTransBase_[index] > 0);
}


uint8 Keyboard::halfTransitions(Key kc) const {
	auto index = static_cast<int>(kc);
	return halfTransBase_[index];
}


void Keyboard::resetHalfTransitions() {
	memset(halfTransBase_, 0, 128u);
}


void Keyboard::clear() {
	keyData_.clear();
}


void Keyboard::keyDownEvent(Key kc) {
	auto index = static_cast<int>(kc);

	// ignore key-repeat events for key state
	if (! downBase_[index]) {
		downBase_[index] = true;
		halfTransBase_[index]++;
	}
}


void Keyboard::keyUpEvent(Key kc) {
	auto index = static_cast<int>(kc);
	downBase_[index] = false;
	halfTransBase_[index]++;
}


} // ns io
} // ns stardazed
