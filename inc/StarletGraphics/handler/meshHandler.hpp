#pragma once

struct MeshCPU;
struct MeshGPU;

struct MeshHandler {
	bool uploadMesh(MeshCPU& meshData, MeshGPU& meshOut);
	void unloadMesh(MeshGPU& mesh);
};