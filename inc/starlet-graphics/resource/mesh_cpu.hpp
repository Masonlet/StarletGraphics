#pragma once

#include "starlet-graphics/resource/resource_cpu.hpp"

#include "starlet-math/vertex.hpp"
#include <vector>

namespace Starlet::Graphics {
  struct MeshCPU : public ResourceCPU<MeshCPU> {
    std::vector<Math::Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int numVertices{ 0 }, numIndices{ 0 }, numTriangles{ 0 };

    bool hasNormals{ false }, hasColours{ false }, hasTexCoords{ false };
    float minY{ 0.0f }, maxY{ 0.0 };

    bool empty() const { return vertices.empty() || indices.empty() || numVertices == 0 || numIndices == 0; }
    void move(MeshCPU&& other) {
      numVertices = other.numVertices;
      numIndices = other.numIndices;
      numTriangles = other.numTriangles;
      vertices = std::move(other.vertices);
      indices = std::move(other.indices);
      hasNormals = other.hasNormals;
      hasColours = other.hasColours;
      hasTexCoords = other.hasTexCoords;
      minY = other.minY;
      maxY = other.maxY;
      other.vertices.clear();
      other.indices.clear();
    }
  };
}