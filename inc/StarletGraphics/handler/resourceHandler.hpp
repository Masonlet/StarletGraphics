#pragma once

template<typename CPUType, typename GPUType>
class ResourceHandler {
public:
  virtual ~ResourceHandler() = default;

  virtual bool upload(CPUType& cpu, GPUType& gpu) = 0;
  virtual void unload(GPUType& gpu) = 0;
};