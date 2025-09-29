#pragma once

struct MeshCPU;
struct MeshGPU;
#include <string>

struct MeshLoader {
	bool loadMesh(const std::string& path , MeshCPU& meshOut);
	bool uploadMesh(MeshCPU& meshData, MeshGPU& meshOut);
	void unloadMesh(MeshGPU& mesh);
};