#pragma once

#include "StarletGraphics/manager/resourceManager.hpp"
#include "StarletGraphics/resource/mesh.hpp"
#include "StarletGraphics/loader/meshLoader.hpp"
#include <map>

class MeshManager : public ResourceManager {
public:
	~MeshManager();

	bool loadAndAddMesh(const std::string& path);
	bool addMesh(const std::string& path, MeshCPU& mesh);

	bool createTriangle(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour);
	bool createSquare(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour);
	bool createCube(const std::string& name, const Vec3<float>& size, const Vec4<float>& vertexColour);

	bool exists(const std::string& name) const override {
		return pathToCPUMeshes.find(name) != pathToCPUMeshes.end()
			  || pathToGPUMeshes.find(name) != pathToGPUMeshes.end();
	}

	bool getMeshCPU(const std::string& path, MeshCPU*& dataOut);
	bool getMeshCPU(const std::string& path, const MeshCPU*& dataOut) const;
	bool getMeshGPU(const std::string& path, MeshGPU*& dataOut);
	bool getMeshGPU(const std::string& path, const MeshGPU*& dataOut) const;

private:
	MeshLoader loader;
	std::map<std::string, MeshCPU> pathToCPUMeshes;
	std::map<std::string, MeshGPU> pathToGPUMeshes;
};