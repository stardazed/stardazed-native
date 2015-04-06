// ------------------------------------------------------------------
// device::Mouse - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_DEVICE_MOUSE_H
#define SD_DEVICE_MOUSE_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "device/Controller.hpp"

#include <array>

namespace stardazed {
namespace device {


class Mouse {
	SD_NOCOPYORMOVE_CLASS(Mouse)
	
	std::array<Button, 16> buttons_;
	
public:
	Mouse() = default;
};


} // ns device
} // ns stardazed

#endif
