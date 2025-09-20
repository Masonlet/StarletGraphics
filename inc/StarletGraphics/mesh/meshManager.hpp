#pragma once

#include "mesh.hpp"
#include "meshLoader.hpp"
#include <string>	
#include <map>

class MeshManager {
public:
	MeshManager() = default;
	~MeshManager();

	void setBasePath(const char* path) { basePath = path; }

	bool loadAndAddMesh(const std::string& path);
	bool addMesh(const std::string& path, MeshCPU& mesh);

	bool createTriangle(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour);
	bool createSquare(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour);
	bool createCube(const std::string& name, const Vec3& size, const Vec4& vertexColour);

	bool findMesh(const std::string& path) const;
	bool getMeshCPU(const std::string& path, MeshCPU*& dataOut);
	bool getMeshCPU(const std::string& path, const MeshCPU*& dataOut) const;
	bool getMeshGPU(const std::string& path, MeshGPU*& dataOut);
	bool getMeshGPU(const std::string& path, const MeshGPU*& dataOut) const;

private:
	MeshLoader loader;
	std::string basePath;
	std::map<std::string, MeshCPU> pathToCPUMeshes;
	std::map<std::string, MeshGPU> pathToGPUMeshes;
};