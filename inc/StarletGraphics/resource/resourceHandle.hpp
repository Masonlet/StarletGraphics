#pragma once

#include <cstdint>

namespace Starlet::Graphics {
	struct ResourceHandle {
		uint32_t id{ 0 };
		bool isValid() const { return id != 0; }
	};
}