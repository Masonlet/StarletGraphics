#pragma once

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