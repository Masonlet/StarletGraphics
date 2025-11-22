#pragma once

#include "starlet-graphics/manager/manager.hpp"
#include "starlet-graphics/handler/mesh_handler.hpp"
#include "starlet-graphics/resource/mesh_cpu.hpp"
#include "starlet-graphics/resource/mesh_gpu.hpp"

#include "starlet-serializer/parser/mesh_parser.hpp"

#include <map>

namespace Starlet::Graphics {
	class MeshManager : public Manager {
	public:
		~MeshManager();

		bool exists(const std::string& name) const override {
			return pathToCPUMeshes.find(name) != pathToCPUMeshes.end()
				|| pathToGPUMeshes.find(name) != pathToGPUMeshes.end();
		}

		bool loadAndAddMesh(const std::string& path);
		bool addMesh(const std::string& path, MeshCPU& mesh);

		MeshCPU* getMeshCPU(const std::string& path);
		const MeshCPU* getMeshCPU(const std::string& path) const;
		MeshGPU* getMeshGPU(const std::string& path);
		const MeshGPU* getMeshGPU(const std::string& path) const;

	private:
		Serializer::MeshParser parser;
		MeshHandler handler;
		std::map<std::string, MeshCPU> pathToCPUMeshes;
		std::map<std::string, MeshGPU> pathToGPUMeshes;
	};
}