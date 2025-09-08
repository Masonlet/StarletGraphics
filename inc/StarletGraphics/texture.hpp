#pragma once

#include <string>
#include <vector>

struct Texture {
  uint32_t id{ 0 };
  int32_t  width{ 0 }, height{ 0 };
  std::vector<uint8_t> pixels;
  uint8_t  pixelSize{ 0 };
  size_t   byteSize{ 0 };

  inline bool empty() const { return width == 0 || height == 0 || pixels.empty() || byteSize == 0; }
  inline void freePixels() { pixels.clear(); byteSize = 0; }

  Texture() = default;
  ~Texture() = default;

  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  Texture(Texture&& other) noexcept { *this = static_cast<Texture&&>(other); }
  Texture& operator=(Texture&& other) noexcept {
    if (this != &other) {
      id = other.id;
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