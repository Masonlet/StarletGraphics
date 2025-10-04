#include "StarletGraphics/handler/textureHandler.hpp"
#include "StarletGraphics/resource/textureCPU.hpp"
#include "StarletGraphics/resource/textureGPU.hpp"

#include "StarletSerializer/parser.hpp"
#include "StarletSerializer/utils/log.hpp"

#include <glad/glad.h>

bool TextureHandler::upload(TextureCPU& cpuTexture, TextureGPU& gpuTexture) {
	return upload(cpuTexture, gpuTexture, true);
}
bool TextureHandler::upload(TextureCPU& cpuTexture, TextureGPU& gpuTexture, bool generateMIPMap) {
  if (cpuTexture.empty()) return error("TextureHandler", "upload", "Attempting to upload empty Texture");

  glGenTextures(1, &gpuTexture.id);
  glBindTexture(GL_TEXTURE_2D, gpuTexture.id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  const GLenum src = (cpuTexture.pixelSize == 4) ? GL_RGBA : GL_RGB;
  const GLint  internal = (cpuTexture.pixelSize == 4) ? GL_RGBA8 : GL_RGB8;
  glTexImage2D(GL_TEXTURE_2D, 0, internal, cpuTexture.width, cpuTexture.height, 0, src, GL_UNSIGNED_BYTE, cpuTexture.pixels.data());

  if (generateMIPMap) glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMIPMap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    if (gpuTexture.id) glDeleteTextures(1, &gpuTexture.id);
    return error("TextureHandler", "upload", "OpenGL error " + std::to_string(err));
  }

  cpuTexture.freePixels();
  return true;
}
bool TextureHandler::upload(TextureCPU(&faces)[6], TextureGPU& cubeOut) {
	return upload(faces, cubeOut, true);
}
bool TextureHandler::upload(TextureCPU(&faces)[6], TextureGPU& cubeOut, bool generateMIPMap) {
  const int w = faces[0].width, h = faces[0].height;
  const uint8_t bpp = faces[0].pixelSize;
  for (int i = 0; i < 6; ++i)
    if (faces[i].pixels.empty() || faces[i].width != w || faces[i].height != h || faces[i].pixelSize != bpp)
      return error("TextureHandler", "upload", "Inconsistent cube faces");

  glGenTextures(1, &cubeOut.id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeOut.id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  const GLenum src = (bpp == 4) ? GL_RGBA : GL_RGB;
  const GLint  internal = (bpp == 4) ? GL_RGBA8 : GL_RGB8;
  const GLenum targets[6] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
  };

  for (int i = 0; i < 6; ++i)
    glTexImage2D(targets[i], 0, internal, w, h, 0, src, GL_UNSIGNED_BYTE, faces[i].pixels.data());

  if (generateMIPMap) glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, generateMIPMap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    if (cubeOut.id) glDeleteTextures(1, &cubeOut.id);
    return error("TextureHandler", "upload", "OpenGL error " + std::to_string(err));
  }

  for (int i = 0; i < 6; ++i)
		faces[i].freePixels();
  return true;
}

void TextureHandler::unload(TextureGPU& texture) {
  if (texture.id) {
    glDeleteTextures(1, &texture.id);
    texture.id = 0;
  }
}
