#pragma once

#include <cstdint>
#include <utility>

class ShaderGPU {
public:
  uint32_t programID{ 0 };
  uint32_t vertexID{ 0 };
  uint32_t fragmentID{ 0 };
  bool     linked{ false };

  bool empty() const { return programID == 0 || !linked; }

  ShaderGPU() = default;
  ~ShaderGPU() = default;

  ShaderGPU(const ShaderGPU&) = delete;
  ShaderGPU& operator=(const ShaderGPU&) = delete;

  ShaderGPU(ShaderGPU&& other) noexcept { *this = std::move(other); }
  ShaderGPU& operator=(ShaderGPU&& other) noexcept {
    if (this != &other) {
      programID = std::move(other.programID);
      vertexID = std::move(other.vertexID);
      fragmentID = std::move(other.fragmentID);
      linked = std::move(other.linked);

      other.programID = other.vertexID = other.fragmentID = 0;
      other.linked = false;
    }
    return *this;
  }
};

