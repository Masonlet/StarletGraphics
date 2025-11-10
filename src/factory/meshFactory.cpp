#include "starlet-graphics/factory/meshFactory.hpp"
#include "starlet-graphics/manager/meshManager.hpp"
#include "starlet-logger/logger.hpp"

#include "starlet-graphics/resource/meshCPU.hpp"

#include "starlet-scene/component/primitive.hpp"
#include "starlet-scene/component/grid.hpp"
#include "starlet-scene/component/transform.hpp"
#include "starlet-scene/component/colour.hpp"

namespace Starlet::Graphics {
  bool MeshFactory::createPrimitiveMesh(const Scene::Primitive& primitive, const Scene::TransformComponent& transform, const Scene::ColourComponent& colour) {
    switch (primitive.type) {
    case Scene::PrimitiveType::Triangle:
      return createTriangle(primitive.name, { transform.size.x, transform.size.y }, colour.colour);
    case Scene::PrimitiveType::Square:
      return createSquare(primitive.name, { transform.size.x, transform.size.y }, colour.colour);
    case Scene::PrimitiveType::Cube:
      return createCube(primitive.name, transform.size, colour.colour);
    default:
      return Logger::error("Renderer", "createPrimitiveMesh", "Invalid primitive: " + primitive.name);
    }
  }

  bool MeshFactory::createGridMesh(const Scene::Grid& grid, const std::string& meshName, const Scene::TransformComponent& transform, const Scene::ColourComponent& colour) {
    switch (grid.type) {
    case Scene::GridType::Square:
      return createSquare(meshName, { transform.size.x, transform.size.y }, colour.colour);
    case Scene::GridType::Cube:
      return createCube(meshName, transform.size, colour.colour);
    default:
      return Logger::error("Renderer", "createGridMesh", "Invalid grid: " + grid.name + ", mesh: " + meshName);
    }
  }

  bool MeshFactory::createTriangle(const std::string& name, const Math::Vec2<float>& size, const Math::Vec4<float>& vertexColour) {
    MeshCPU info;

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

    return meshManager.addMesh(name, info)
      ? Logger::debugLog("Primitive", "createTriangle", "Added triangle: " + name)
      : Logger::error("Primitive", "createTriangle", "Failed to create triangle " + name);
  }

  bool MeshFactory::createSquare(const std::string& name, const Math::Vec2<float>& size, const Math::Vec4<float>& vertexColour) {
    MeshCPU info;
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

    return meshManager.addMesh(name, info)
      ? Logger::debugLog("Primitive", "createSquare", "Added square: " + name)
      : Logger::error("Primitive", "createSquare", "Failed to create square " + name);
  }

  bool MeshFactory::createCube(const std::string& name, const Math::Vec3<float>& size, const Math::Vec4<float>& vertexColour) {
    MeshCPU info;
    constexpr int vertexCount = 36;

    info.numVertices = vertexCount;
    info.numIndices = vertexCount;
    info.numTriangles = 12;

    info.vertices.resize(vertexCount);
    info.indices.resize(vertexCount);

    float x = 0.5f * size.x;
    float y = 0.5f * size.y;
    float z = 0.5f * size.z;

    Math::Vec3<float> positions[8] = {
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

    return meshManager.addMesh(name, info)
      ? Logger::debugLog("Primitive", "createCube", "Added cube: " + name)
      : Logger::error("Primitive", "createCube", "Failed to create cube " + name);
  }
}