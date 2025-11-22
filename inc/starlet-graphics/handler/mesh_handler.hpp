#pragma once

#include "starlet-graphics/handler/resource_handler.hpp"

namespace Starlet::Graphics {
	struct MeshCPU;
	struct MeshGPU;

	struct MeshHandler : public ResourceHandler<MeshCPU, MeshGPU> {
		bool upload(MeshCPU& cpu, MeshGPU& gpu) override;
		void unload(MeshGPU& gpu) override;
	};
}