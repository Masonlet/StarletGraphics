#pragma once

#include "mesh.hpp"
#include "meshLoader.hpp"
#include <string>	
#include <map>

class MeshManager {
public:
	MeshManager() = default;
	~MeshManager();

	void setBasePath(const std::string& path) { basePath = path; }

	bool addMesh(const std::string& path);
	bool addMesh(const std::string& path, Mesh& mesh);

	bool createTriangle(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour);
	bool createSquare(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour);
	bool createCube(const std::string& name, const Vec3& size, const Vec4& vertexColour);

	bool findMesh(const std::string& path) const;
	bool getMesh(const std::string& path, Mesh*& dataOut);
	bool getMesh(const std::string& path, const Mesh*& dataOut) const;

private:
	MeshLoader loader;
	std::string basePath;
	std::map<std::string, Mesh> pathToMeshes;
};