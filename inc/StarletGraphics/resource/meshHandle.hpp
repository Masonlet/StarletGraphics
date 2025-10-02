#pragma once

#include <cstdint>

struct MeshHandle {
	uint32_t id{ 0 };

	bool isValid() const { return id != 0; }
};