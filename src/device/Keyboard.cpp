// ------------------------------------------------------------------
// device::Keyboard - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "device/Keyboard.hpp"

namespace stardazed {
namespace device {


constexpr uint16_t keyIndexForKey(Key k) {
	return static_cast<uint16_t>(k);
}


KeyboardSnapshot::KeyboardSnapshot(std::bitset<128> keys)
: pressed_(keys)
{}


bool KeyboardSnapshot::isPressed(Key k) const {
	return pressed_.test(keyIndexForKey(k));
}


const KeyboardSnapshot Keyboard::snapshot() const {
	return { pressed_ };
}


bool Keyboard::isPressed(Key k) const {
	return pressed_.test(keyIndexForKey(k));
}


void Keyboard::press(Key k) {
	pressed_.set(keyIndexForKey(k));
}


void Keyboard::release(Key k) {
	pressed_.reset(keyIndexForKey(k));
}


} // ns device
} // ns stardazed
