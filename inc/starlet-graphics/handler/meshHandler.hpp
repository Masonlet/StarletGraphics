#pragma once

#include "starlet-graphics/handler/resourceHandler.hpp"

namespace Starlet::Graphics {
	struct MeshCPU;
	struct MeshGPU;

	struct MeshHandler : public ResourceHandler<MeshCPU, MeshGPU> {
		bool upload(MeshCPU& cpu, MeshGPU& gpu) override;
		void unload(MeshGPU& gpu) override;
	};
}