#include "StarletGraphics/mesh/meshManager.hpp"
#include "StarletParsers/utils/log.hpp"

MeshManager::~MeshManager() {
	for (std::map<std::string, Mesh>::iterator it = pathToMeshes.begin(); it != pathToMeshes.end(); ++it)
		loader.unloadMesh(it->second);
}

bool MeshManager::loadAndAddMesh(const std::string& path) {
	if (findMesh(path)) return true;

	Mesh mesh;
	if (!loader.loadMesh(basePath + "/models/" + path, mesh))
		return error("MeshManager", "loadAndAddMesh", "Could not load mesh from " + path);

	if (!loader.uploadMesh(mesh))
		return error("MeshManager", "loadAndAddMesh", "Could not upload mesh from: " + path);

	pathToMeshes[path] = std::move(mesh);
	return true;
}
bool MeshManager::addMesh(const std::string& path, Mesh& mesh) {
	if (findMesh(path)) return true;
	if (mesh.empty()) return error("MeshManager", "addMesh", "Trying to add an empty mesh");

	if (!loader.uploadMesh(mesh))
		return error("MeshManager", "addMesh", "Could not upload mesh from: " + path);

	pathToMeshes[path] = std::move(mesh);
	return true;
}

bool MeshManager::createTriangle(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour) {
	Mesh info;

	info.numVertices = 3;
	info.vertices.resize(3);
	info.vertices[0].pos = { -0.5f * size.x, -0.5f * size.y, 0.0f };
	info.vertices[1].pos = { 0.5f * size.x, -0.5f * size.y, 0.0f };
	info.vertices[2].pos = { 0.0f,           0.5f * size.y, 0.0f };
	for (int i = 0; i < 3; ++i)	info.vertices[i].col = vertexColour;

	info.numIndices = 3;
	info.numTriangles = 1;
	info.indices.resize(3);
	info.indices[0] = 0;
	info.indices[1] = 1;
	info.indices[2] = 2;

	return addMesh(name, info) ? true : error("Primitive", "createTriangle", "Failed to create triangle " + name);
}
bool MeshManager::createSquare(const std::string& name, const Vec2<float>& size, const Vec4& vertexColour) {
	Mesh info;
	info.numVertices = 6;
	info.numIndices = 6;
	info.numTriangles = 2;

	info.vertices.resize(6);
	info.indices.resize(6);

	const float halfX = 0.5f * size.x;
	const float halfY = 0.5f * size.y;

	// First triangle
	info.vertices[0].pos = { -halfX, -halfY, 0.0f };
	info.vertices[1].pos = { halfX, -halfY, 0.0f };
	info.vertices[2].pos = { halfX,  halfY, 0.0f };

	// Second triangle
	info.vertices[3].pos = { -halfX, -halfY, 0.0f };
	info.vertices[4].pos = { halfX,  halfY, 0.0f };
	info.vertices[5].pos = { -halfX,  halfY, 0.0f };

	for (int i = 0; i < 6; ++i) {
		info.vertices[i].col = vertexColour;
		info.indices[i] = i;
	}

	return addMesh(name, info) ? true : error("Primitive", "createSquare", "Failed to create square " + name);
}
bool MeshManager::createCube(const std::string& name, const Vec3& size, const Vec4& vertexColour) {
	Mesh info;
	constexpr int vertexCount = 36;

	info.numVertices = vertexCount;
	info.numIndices = vertexCount;
	info.numTriangles = 12;

	info.vertices.resize(vertexCount);
	info.indices.resize(vertexCount);

	float x = 0.5f * size.x;
	float y = 0.5f * size.y;
	float z = 0.5f * size.z;

	Vec3 positions[8] = {
		{-x, -y, -z}, { x, -y, -z}, { x,  y, -z}, { -x,  y, -z}, // Back
		{-x, -y,  z}, { x, -y,  z}, { x,  y,  z}, { -x,  y,  z}  // Front
	};

	unsigned int faces[6][6] = {
		{4, 5, 6, 4, 6, 7}, // Front
		{1, 0, 3, 1, 3, 2}, // Back
		{0, 4, 7, 0, 7, 3}, // Left
		{5, 1, 2, 5, 2, 6}, // Right
		{3, 7, 6, 3, 6, 2}, // Top
		{0, 1, 5, 0, 5, 4}  // Bottom
	};

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			int idx = i * 6 + j;

			info.vertices[idx].pos = positions[faces[i][j]];
			info.vertices[idx].col = vertexColour;
			info.indices[idx] = idx;
		}
	}

	return addMesh(name, info) ? true : error("Primitive", "createCube", "Failed to create cube " + name);
}

bool MeshManager::findMesh(const std::string& name) const {
	return pathToMeshes.find(name) != pathToMeshes.end();
}
bool MeshManager::getMesh(const std::string& name, Mesh*& data) {
	std::map<std::string, Mesh>::iterator it = pathToMeshes.find(name);
	if (it == pathToMeshes.end()) return false;
	data = &it->second;
	return true;
}
bool MeshManager::getMesh(const std::string& name, const Mesh*& data) const {
	std::map<std::string, Mesh>::const_iterator it = pathToMeshes.find(name);
	if (it == pathToMeshes.end()) return false;
	data = &it->second;
	return true;
}
