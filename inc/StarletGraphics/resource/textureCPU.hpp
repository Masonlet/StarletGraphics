#pragma once

#include "StarletGraphics/resource/resourceCPU.hpp"

#include <vector>

namespace Starlet::Graphics {
  struct TextureCPU : public ResourceCPU<TextureCPU> {
    int32_t width{ 0 }, height{ 0 };
    std::vector<uint8_t> pixels;
    uint8_t  pixelSize{ 0 };
    size_t   byteSize{ 0 };

    void freePixels() { pixels.clear(); byteSize = 0; }
    bool empty() const { return width == 0 || height == 0 || pixels.empty() || byteSize == 0; }
    void move(TextureCPU&& other) {
      width = other.width;
      height = other.height;
      pixels = std::move(other.pixels);
      pixelSize = other.pixelSize;
      byteSize = other.byteSize;
      other.pixels.clear();
      other.byteSize = 0;
    }
  };
}