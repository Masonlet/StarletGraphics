#pragma once

#include "StarletGraphics/resource/resourceCPU.hpp"

#include <string>

namespace Starlet::Graphics {
  struct ShaderCPU : ResourceCPU<ShaderCPU> {
    std::string vertexSource;
    std::string fragmentSource;
    std::string vertexPath;
    std::string fragmentPath;
    bool        valid{ false };

    bool empty() const { return vertexSource.empty() || fragmentSource.empty(); }
    void move(ShaderCPU&& other) {
      vertexSource = std::move(other.vertexSource);
      fragmentSource = std::move(other.fragmentSource);
      vertexPath = std::move(other.vertexPath);
      fragmentPath = std::move(other.fragmentPath);

      valid = other.valid;
      other.valid = false;
    }
  };
}