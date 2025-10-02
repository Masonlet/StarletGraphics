#pragma once

struct MeshCPU;
struct MeshGPU;
#include <string>

struct MeshHandler {
	bool uploadMesh(MeshCPU& meshData, MeshGPU& meshOut);
	void unloadMesh(MeshGPU& mesh);
};