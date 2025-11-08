#include "StarletGraphics/manager/meshManager.hpp"
#include "StarletLogger/logger.hpp"

#include "StarletSerializer/data/meshData.hpp"

namespace Starlet::Graphics {
	MeshManager::~MeshManager() {
		for (std::map<std::string, MeshGPU>::iterator it = pathToGPUMeshes.begin(); it != pathToGPUMeshes.end(); ++it)
			handler.unload(it->second);
	}

	bool MeshManager::loadAndAddMesh(const std::string& path) {
		if (exists(path)) return Logger::debugLog("MeshManager", "addMesh", "Mesh already exists: " + path);

		Serializer::MeshData data;
		if(!parser.parse(basePath + path, data))
			return Logger::error("MeshManager", "loadAndAddMesh", "Could not load mesh from " + path);

		MeshCPU meshCPU;
		meshCPU.hasColours = data.hasColours;
		meshCPU.hasNormals = data.hasNormals;
		meshCPU.hasTexCoords = data.hasTexCoords;
		meshCPU.numIndices = data.numIndices;
		meshCPU.numVertices = data.numVertices;
		meshCPU.numTriangles = data.numTriangles;
		meshCPU.indices = std::move(data.indices);
		meshCPU.vertices = std::move(data.vertices);
		meshCPU.minY = data.minY;
		meshCPU.maxY = data.maxY;

		MeshGPU meshGPU;
		if (!handler.upload(meshCPU, meshGPU))
			return Logger::error("MeshManager", "loadAndAddMesh", "Could not upload mesh from: " + path);

		pathToGPUMeshes[path] = std::move(meshGPU);
		pathToCPUMeshes[path] = std::move(meshCPU);
		return Logger::debugLog("MeshManager", "addMesh", "Added mesh: " + path);
	}
	bool MeshManager::addMesh(const std::string& path, MeshCPU& meshCPU) {
		if (exists(path)) return true;
		if (meshCPU.empty()) return Logger::error("MeshManager", "addMesh", "Trying to add an empty mesh");

		MeshGPU meshGPU;
		if(!handler.upload(meshCPU, meshGPU))
			return Logger::error("MeshManager", "addMesh", "Could not upload mesh from: " + path);

		pathToGPUMeshes[path] = std::move(meshGPU);
		pathToCPUMeshes[path] = std::move(meshCPU);
		return Logger::debugLog("MeshManager", "addMesh", "Added mesh: " + path);
	}

	MeshGPU* MeshManager::getMeshGPU(const std::string& name) {
		std::map<std::string, MeshGPU>::iterator it = pathToGPUMeshes.find(name);
		if (it == pathToGPUMeshes.end()) return nullptr;
		return &it->second;
	}
	const MeshGPU* MeshManager::getMeshGPU(const std::string& name) const {
		std::map<std::string, MeshGPU>::const_iterator it = pathToGPUMeshes.find(name);
		if (it == pathToGPUMeshes.end()) return nullptr;
		return &it->second;
	}

	MeshCPU* MeshManager::getMeshCPU(const std::string& name) {
		std::map<std::string, MeshCPU>::iterator it = pathToCPUMeshes.find(name);
		if (it == pathToCPUMeshes.end()) return nullptr;
		return &it->second;
	}
	const MeshCPU* MeshManager::getMeshCPU(const std::string& name) const {
		std::map<std::string, MeshCPU>::const_iterator it = pathToCPUMeshes.find(name);
		if (it == pathToCPUMeshes.end()) return nullptr;
		return &it->second;
	}
}