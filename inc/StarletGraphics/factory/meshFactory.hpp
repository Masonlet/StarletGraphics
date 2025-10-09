#pragma once

template<typename T> struct Vec2;
template<typename T> struct Vec3;
template<typename T> struct Vec4;

struct Primitive;
struct Grid;

struct TransformComponent;
struct ColourComponent;

class MeshManager;

#include <string>

class MeshFactory {
public:
	MeshFactory(MeshManager& mm) : meshManager(mm) {};

	bool createPrimitiveMesh(const Primitive& primitive, const TransformComponent& transform, const ColourComponent& colour);
	bool createGridMesh(const Grid& grid, const std::string& meshName, const TransformComponent& transform, const ColourComponent& colour);

private:
	bool createTriangle(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour);
	bool createSquare(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour);
	bool createCube(const std::string& name, const Vec3<float>& size, const Vec4<float>& vertexColour);

	MeshManager& meshManager;
};