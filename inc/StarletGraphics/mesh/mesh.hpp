#pragma once

#include "StarletMath/vertex.hpp"
#include <string>
#include <vector>

struct Mesh {
  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  unsigned int numVertices{ 0 }, numIndices{ 0 }, numTriangles{ 0 };

  bool hasNormals{ false }, hasColours{ false }, hasTexCoords{ false };
  float minY{ 0.0f }, maxY{ 0.0 };

  unsigned int VAOID{ 0 }, VertexBufferID{ 0 }, IndexBufferID{ 0 };
  unsigned int VertexBuffer_Start_Index{ 0 }, IndexBuffer_Start_Index{ 0 };

  inline bool empty() const { return vertices.empty() || indices.empty() || numVertices == 0 || numIndices == 0; }

  Mesh() = default;
  ~Mesh() = default;

  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;

  Mesh(Mesh&& other) noexcept { *this = static_cast<Mesh&&>(other); }
  Mesh& operator=(Mesh&& other) noexcept {
    if (this != &other) {
      VAOID = other.VAOID;
      VertexBufferID = other.VertexBufferID;
      IndexBufferID = other.IndexBufferID;
      VertexBuffer_Start_Index = other.VertexBuffer_Start_Index;
      IndexBuffer_Start_Index = other.IndexBuffer_Start_Index;

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