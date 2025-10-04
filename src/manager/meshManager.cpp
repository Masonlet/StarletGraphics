#include "StarletGraphics/manager/meshManager.hpp"
#include "StarletSerializer/utils/log.hpp"

MeshManager::~MeshManager() {
	for (std::map<std::string, MeshGPU>::iterator it = pathToGPUMeshes.begin(); it != pathToGPUMeshes.end(); ++it)
		handler.unload(it->second);
}

bool MeshManager::loadAndAddMesh(const std::string& path) {
	if (exists(path)) return debugLog("MeshManager", "addMesh", "Mesh already exists: " + path);

	MeshCPU meshCPU;
	if(!parser.parsePlyMesh(basePath + path, meshCPU))
		return error("MeshManager", "loadAndAddMesh", "Could not load mesh from " + path);

	MeshGPU meshGPU;
	if (!handler.upload(meshCPU, meshGPU))
		return error("MeshManager", "loadAndAddMesh", "Could not upload mesh from: " + path);

	pathToGPUMeshes[path] = std::move(meshGPU);
	pathToCPUMeshes[path] = std::move(meshCPU);
	return debugLog("MeshManager", "addMesh", "Added mesh: " + path);
}
bool MeshManager::addMesh(const std::string& path, MeshCPU& meshCPU) {
	if (exists(path)) return true;
	if (meshCPU.empty()) return error("MeshManager", "addMesh", "Trying to add an empty mesh");

	MeshGPU meshGPU;
	if(!handler.upload(meshCPU, meshGPU))
		return error("MeshManager", "addMesh", "Could not upload mesh from: " + path);

	pathToGPUMeshes[path] = std::move(meshGPU);
	pathToCPUMeshes[path] = std::move(meshCPU);
	return debugLog("MeshManager", "addMesh", "Added mesh: " + path);
}

bool MeshManager::getMeshGPU(const std::string& name, MeshGPU*& data) {
	std::map<std::string, MeshGPU>::iterator it = pathToGPUMeshes.find(name);
	if (it == pathToGPUMeshes.end()) return false;
	data = &it->second;
	return true;
}
bool MeshManager::getMeshGPU(const std::string& name, const MeshGPU*& data) const {
	std::map<std::string, MeshGPU>::const_iterator it = pathToGPUMeshes.find(name);
	if (it == pathToGPUMeshes.end()) return false;
	data = &it->second;
	return true;
}

bool MeshManager::getMeshCPU(const std::string& name, MeshCPU*& data) {
	std::map<std::string, MeshCPU>::iterator it = pathToCPUMeshes.find(name);
	if (it == pathToCPUMeshes.end()) return false;
	data = &it->second;
	return true;
}
bool MeshManager::getMeshCPU(const std::string& name, const MeshCPU*& data) const {
	std::map<std::string, MeshCPU>::const_iterator it = pathToCPUMeshes.find(name);
	if (it == pathToCPUMeshes.end()) return false;
	data = &it->second;
	return true;
}