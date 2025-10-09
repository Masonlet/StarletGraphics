#include "StarletGraphics/factory/meshFactory.hpp"
#include "StarletGraphics/manager/meshManager.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletGraphics/resource/meshCPU.hpp"

#include "StarletScene/components/primitive.hpp"
#include "StarletScene/components/grid.hpp"
#include "StarletScene/components/transform.hpp"
#include "StarletScene/components/colour.hpp"

bool MeshFactory::createPrimitiveMesh(const Primitive& primitive, const TransformComponent& transform, const ColourComponent& colour) {
  switch (primitive.type) {
  case PrimitiveType::Triangle:
    return createTriangle(primitive.name, { transform.size.x, transform.size.y }, colour.colour);
  case PrimitiveType::Square:
    return createSquare(primitive.name, { transform.size.x, transform.size.y }, colour.colour);
  case PrimitiveType::Cube:
    return createCube(primitive.name, transform.size, colour.colour);
  default:
    return error("Renderer", "createPrimitiveMesh", "Invalid primitive: " + primitive.name);
  }
}

bool MeshFactory::createGridMesh(const Grid& grid, const std::string& meshName, const TransformComponent& transform, const ColourComponent& colour) {
  switch (grid.type) {
  case GridType::Square:
    return createSquare(meshName, { transform.size.x, transform.size.y }, colour.colour);
  case GridType::Cube:
    return createCube(meshName, transform.size, colour.colour);
  default:
    return error("Renderer", "createGridMesh", "Invalid grid: " + grid.name + ", mesh: " + meshName);
  }
}

bool MeshFactory::createTriangle(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour) {
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
    ? debugLog("Primitive", "createTriangle", "Added triangle: " + name)
    : error("Primitive", "createTriangle", "Failed to create triangle " + name);
}

bool MeshFactory::createSquare(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour) {
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
    ? debugLog("Primitive", "createSquare", "Added square: " + name)
    : error("Primitive", "createSquare", "Failed to create square " + name);
}

bool MeshFactory::createCube(const std::string& name, const Vec3<float>& size, const Vec4<float>& vertexColour) {
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

  Vec3<float> positions[8] = {
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
    ? debugLog("Primitive", "createCube", "Added cube: " + name)
    : error("Primitive", "createCube", "Failed to create cube " + name);
}