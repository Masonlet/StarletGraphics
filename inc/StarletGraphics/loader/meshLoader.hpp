#pragma once

#include "StarletSerializer/parser.hpp"
#include "StarletGraphics/handler/meshHandler.hpp"

#include <string>

struct MeshCPU;
struct MeshGPU;

class MeshLoader {
public:
	bool loadMesh(MeshCPU& mesh, const std::string& path);
	bool uploadMesh(MeshCPU& meshCPU, MeshGPU& meshGPU);
	void unloadMesh(MeshGPU& mesh);

private:
	Parser parser;
	MeshHandler handler;
};