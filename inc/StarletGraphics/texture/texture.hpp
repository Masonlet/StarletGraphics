#pragma once

#include <string>
#include <vector>

struct TextureGPU {
  uint32_t id{ 0 };

  TextureGPU() = default;
  ~TextureGPU() = default;

  TextureGPU(const TextureGPU&) = delete;
  TextureGPU& operator=(const TextureGPU&) = delete;
  TextureGPU(TextureGPU&& other) noexcept { *this = static_cast<TextureGPU&&>(other); }
  TextureGPU& operator=(TextureGPU&& other) noexcept {
    if (this != &other) {
      id = other.id;
      other.id = 0;
    }
    return *this;
  }
};

struct TextureCPU {
  int32_t width{ 0 }, height{ 0 };
  std::vector<uint8_t> pixels;
  uint8_t  pixelSize{ 0 };
  size_t   byteSize{ 0 };

  inline bool empty() const { return width == 0 || height == 0 || pixels.empty() || byteSize == 0; }
  inline void freePixels() { pixels.clear(); byteSize = 0; }

  TextureCPU() = default;
  ~TextureCPU() = default;

  TextureCPU(const TextureCPU&) = delete;
  TextureCPU& operator=(const TextureCPU&) = delete;

  TextureCPU(TextureCPU&& other) noexcept { *this = static_cast<TextureCPU&&>(other); }
  TextureCPU& operator=(TextureCPU&& other) noexcept {
    if (this != &other) {
      width = other.width;
      height = other.height;
      pixels = other.pixels;
      pixelSize = other.pixelSize;
      byteSize = other.byteSize;
      other.pixels.clear();
      other.byteSize = 0;
    }
    return *this;
	}
};
