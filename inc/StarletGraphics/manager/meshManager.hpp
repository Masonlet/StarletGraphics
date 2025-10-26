#pragma once

#include "StarletGraphics/manager/manager.hpp"
#include "StarletGraphics/handler/meshHandler.hpp"
#include "StarletGraphics/resource/meshCPU.hpp"
#include "StarletGraphics/resource/meshGPU.hpp"

#include "StarletSerializer/parser/plyParser.hpp"

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
		Serializer::PlyParser parser;
		MeshHandler handler;
		std::map<std::string, MeshCPU> pathToCPUMeshes;
		std::map<std::string, MeshGPU> pathToGPUMeshes;
	};
}