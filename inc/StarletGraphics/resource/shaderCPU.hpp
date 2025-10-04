#pragma once

#include <string>

struct ShaderCPU {
  std::string vertexSource;
  std::string fragmentSource;
  std::string vertexPath;
  std::string fragmentPath;
  bool        valid { false };

  bool empty() const { return vertexSource.empty() || fragmentSource.empty(); }

  ShaderCPU() = default;
  ~ShaderCPU() = default;

  ShaderCPU(const ShaderCPU&) = delete;
  ShaderCPU& operator=(const ShaderCPU&) = delete;

  ShaderCPU(ShaderCPU&& other) noexcept { *this = std::move(other); }
  ShaderCPU& operator=(ShaderCPU&& other) noexcept {
    if (this != &other) {
      vertexSource = std::move(other.vertexSource);
      fragmentSource = std::move(other.fragmentSource);
      vertexPath = std::move(other.vertexPath);
      fragmentPath = std::move(other.fragmentPath);

      valid = other.valid;
      other.valid = false;
    }
    return *this;
  }
};