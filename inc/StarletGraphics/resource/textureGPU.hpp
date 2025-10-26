#pragma once

#include <cstdint>

namespace Starlet::Graphics {
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
}
