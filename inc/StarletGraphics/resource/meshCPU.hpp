#pragma once

#include "StarletMath/vertex.hpp"
#include <vector>

struct MeshCPU {
  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  unsigned int numVertices{ 0 }, numIndices{ 0 }, numTriangles{ 0 };

  bool hasNormals{ false }, hasColours{ false }, hasTexCoords{ false };
  float minY{ 0.0f }, maxY{ 0.0 };

  inline bool empty() const { return vertices.empty() || indices.empty() || numVertices == 0 || numIndices == 0; }

  MeshCPU() = default;
  ~MeshCPU() = default;

  MeshCPU(const MeshCPU&) = delete;
  MeshCPU& operator=(const MeshCPU&) = delete;

  MeshCPU(MeshCPU&& other) noexcept { *this = static_cast<MeshCPU&&>(other); }
  MeshCPU& operator=(MeshCPU&& other) noexcept {
    if (this != &other) {
      numVertices = other.numVertices;
      numIndices = other.numIndices;
      numTriangles = other.numTriangles;
      vertices = other.vertices;
      indices = other.indices;
      hasNormals = other.hasNormals;
      hasColours = other.hasColours;
      hasTexCoords = other.hasTexCoords;
      minY = other.minY;
      maxY = other.maxY;
      other.vertices.clear();
      other.indices.clear();
    }
    return *this;
	}
};