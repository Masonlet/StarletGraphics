#pragma once

#include <utility>

namespace Starlet::Graphics {
  template<typename T>
  struct ResourceCPU {
    ResourceCPU() = default;
    ~ResourceCPU() = default;

    ResourceCPU(const ResourceCPU&) = delete;
    ResourceCPU& operator=(const ResourceCPU&) = delete;

    ResourceCPU(ResourceCPU&& other) noexcept { *this = static_cast<ResourceCPU&&>(other); }
    ResourceCPU& operator=(ResourceCPU&& other) noexcept {
      if (this != &other) static_cast<T*>(this)->move(std::move(static_cast<T&>(other)));
      return *this;
    }

    bool empty() const { return static_cast<const T*>(this)->empty(); }
  };
}