#include "StarletGraphics/loader/meshLoader.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletGraphics/resource/meshCPU.hpp"
#include "StarletGraphics/resource/meshGPU.hpp"

bool MeshLoader::loadMesh(MeshCPU& mesh, const std::string& path) {
	if (!parser.parsePlyMesh(path, mesh))
		return error("MeshManager", "loadAndAddMesh", "Could not load mesh from " + path);

	return true;
}
bool MeshLoader::uploadMesh(MeshCPU& meshCPU, MeshGPU& meshGPU) {
	if (!handler.uploadMesh(meshCPU, meshGPU))
		return error("MeshManager", "loadAndAddMesh", "Could not upload mesh ");

	return true;
}
void MeshLoader::unloadMesh(MeshGPU& mesh) {
	handler.unloadMesh(mesh);
}