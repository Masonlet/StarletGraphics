#pragma once

#include "StarletGraphics/handler/resourceHandler.hpp"

struct MeshCPU;
struct MeshGPU;

struct MeshHandler : public ResourceHandler<MeshCPU, MeshGPU> {
	bool upload(MeshCPU& cpu, MeshGPU& gpu) override;
	void unload(MeshGPU& gpu) override;
};