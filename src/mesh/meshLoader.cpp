#include "StarletGraphics/mesh/mesh.hpp"
#include "StarletGraphics/mesh/meshLoader.hpp"
#include "StarletGraphics/mesh/parsers/plyParser.hpp"
#include "StarletParsers/utils/log.hpp"
#include <glad/glad.h>

bool MeshLoader::loadMesh(const std::string& path, MeshCPU& mesh) {
  if (!mesh.empty()) return error("MeshLoader", "loadMesh", "Loading non-empty mesh: " + path);
  return parsePlyMesh(path, mesh) ? true : error("MeshLoader", "loadMesh", "Failed to parse mesh file: " + path);
}

bool MeshLoader::uploadMesh(MeshCPU& meshData, MeshGPU& meshOut) {
  if (meshData.empty()) return error("MeshLoader", "uploadMesh", "Invalid mesh data");

  meshOut.numVertices = meshData.numVertices;
  meshOut.numIndices = meshData.numIndices;

  //Create a VAO (Vertex Array Object), which will keep track of all the 'state' needed to draw from this buffer
  glGenVertexArrays(1, &(meshOut.VAOID)); //Ask OpenGL for a new buffer ID
  glBindVertexArray(meshOut.VAOID);       //Bind the buffer: aka "make this the 'current' VAO buffer

  //Now ANY state that is related to vertex or index buffer and vertex attribute layout, is stored in the 'state' of the VAO
  glGenBuffers(1, &(meshOut.VertexBufferID));
  glBindBuffer(GL_ARRAY_BUFFER, meshOut.VertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshOut.numVertices, meshData.vertices.data(), GL_STATIC_DRAW);

  //Copy the index buffer into the video card to create an index buffer
  glGenBuffers(1, &(meshOut.IndexBufferID));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshOut.IndexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshOut.numIndices, meshData.indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) return error("MeshLoader", "UploadToGPU", "OpenGL error " + std::to_string(err));

  meshData.vertices.clear();
  meshData.indices.clear();
  return true;
}

void MeshLoader::unloadMesh(MeshGPU& mesh) {
  if (glIsVertexArray(mesh.VAOID))     glDeleteVertexArrays(1, &mesh.VAOID);
  if (glIsBuffer(mesh.VertexBufferID)) glDeleteBuffers(1, &mesh.VertexBufferID);
  if (glIsBuffer(mesh.IndexBufferID))  glDeleteBuffers(1, &mesh.IndexBufferID);
  mesh.VAOID = mesh.VertexBufferID = mesh.IndexBufferID = 0;
  mesh.numVertices = mesh.numIndices = 0;
}
