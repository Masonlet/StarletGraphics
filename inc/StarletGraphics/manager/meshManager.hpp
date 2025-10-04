#pragma once

#include "StarletGraphics/manager/manager.hpp"

#include "StarletGraphics/resource/meshCPU.hpp"
#include "StarletGraphics/resource/meshGPU.hpp"
#include "StarletGraphics/loader/meshLoader.hpp"

#include <map>

class MeshManager : public Manager {
public:
	~MeshManager();

	bool exists(const std::string& name) const override {
		return pathToCPUMeshes.find(name) != pathToCPUMeshes.end()
			|| pathToGPUMeshes.find(name) != pathToGPUMeshes.end();
	}

	bool loadAndAddMesh(const std::string& path);
	bool addMesh(const std::string& path, MeshCPU& mesh);

	bool getMeshCPU(const std::string& path, MeshCPU*& dataOut);
	bool getMeshCPU(const std::string& path, const MeshCPU*& dataOut) const;
	bool getMeshGPU(const std::string& path, MeshGPU*& dataOut);
	bool getMeshGPU(const std::string& path, const MeshGPU*& dataOut) const;

private:
	MeshLoader loader;
	std::map<std::string, MeshCPU> pathToCPUMeshes;
	std::map<std::string, MeshGPU> pathToGPUMeshes;
};