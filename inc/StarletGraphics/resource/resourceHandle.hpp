#pragma once

#include <cstdint>

struct ResourceHandle {
	uint32_t id{ 0 };
	bool isValid() const { return id != 0; }
};