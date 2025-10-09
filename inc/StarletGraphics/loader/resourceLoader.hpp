#pragma once

template<typename T> struct Vec2;
template<typename T> struct Vec3;
template<typename T> struct Vec4;

struct Primitive;
struct Model;
struct TextureData;
struct Grid;

struct TransformComponent;
struct ColourComponent;

class Scene;
class SceneManager;
class ResourceManager;

#include <vector>
#include <string>

class ResourceLoader {
public:
	ResourceLoader(ResourceManager& rm) : resourceManager(rm){};

	bool loadMeshes(const std::vector<Model*>& models);
	bool loadTextures(const std::vector<TextureData*>& textures);

	bool processTextureConnections(Scene& scene);
	bool processPrimitives(SceneManager& sm);
	bool processGrids(SceneManager& sm);

private:
	bool createTriangle(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour);
	bool createSquare(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour);
	bool createCube(const std::string& name, const Vec3<float>& size, const Vec4<float>& vertexColour);

	bool createPrimitiveMesh(const Primitive& primitive, const TransformComponent& transform, const ColourComponent& colour);
	bool createGridMesh(const Grid& grid, const std::string& meshName, const TransformComponent& transform, const ColourComponent& colour);

	ResourceManager& resourceManager;
};