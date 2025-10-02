#pragma once

#include "StarletMath/vertex.hpp"
#include <string>
#include <vector>

struct MeshGPU {
  unsigned int VAOID{ 0 }, VertexBufferID{ 0 }, IndexBufferID{ 0 };
  unsigned int numVertices{ 0 }, numIndices{ 0 };
  unsigned int VertexBuffer_Start_Index{ 0 }, IndexBuffer_Start_Index{ 0 };

  MeshGPU() = default;
  ~MeshGPU() = default;

  MeshGPU(const MeshGPU&) = delete;
  MeshGPU& operator=(const MeshGPU&) = delete;

  MeshGPU(MeshGPU&& other) noexcept { *this = static_cast<MeshGPU&&>(other); }
  MeshGPU& operator=(MeshGPU&& other) noexcept {
    if (this != &other) {
      VAOID = other.VAOID;
      VertexBufferID = other.VertexBufferID;
      IndexBufferID = other.IndexBufferID;

      numVertices = other.numVertices;
      numIndices = other.numIndices;

      VertexBuffer_Start_Index = other.VertexBuffer_Start_Index;
      IndexBuffer_Start_Index = other.IndexBuffer_Start_Index;
      
      other.VAOID = 0;
      other.VertexBufferID = 0;
      other.IndexBufferID = 0;
    }
    return *this;
  }
};

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