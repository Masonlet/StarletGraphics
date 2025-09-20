#pragma once

struct MeshCPU;
struct MeshGPU;
#include <string>

struct MeshLoader {
	bool loadMesh(const std::string& path, MeshCPU& mesh);
	bool uploadMesh(MeshCPU& meshData, MeshGPU& meshOut);
	void unloadMesh(MeshGPU& mesh);
};