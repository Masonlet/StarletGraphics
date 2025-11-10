#pragma once

#include "starlet-graphics/manager/meshManager.hpp"
#include "starlet-graphics/factory/meshFactory.hpp"
#include "starlet-graphics/manager/textureManager.hpp"

#include "starlet-graphics/resource/resourceHandle.hpp"

#include <unordered_map>
#include <cstdint>
#include <string>

namespace Starlet {
	namespace Scene {
		struct Model;
		struct TextureData;

		struct MeshGPU;
		struct MeshCPU;

		class Scene;
		class SceneManager;
	}

	namespace Graphics {
		class ResourceManager {
		public:
			ResourceManager();
			~ResourceManager() = default;

			void setBasePath(const std::string& path);

			ResourceHandle addMesh(const std::string& path);
			bool hasMesh(const std::string& path) const;
			bool hasMesh(ResourceHandle handle) const;
			ResourceHandle getMeshHandle(const std::string& path) const;

			ResourceHandle addTexture(const std::string& name, unsigned int textureID);
			bool hasTexture(const std::string& name) const;
			bool hasTexture(ResourceHandle handle) const;
			ResourceHandle getTextureHandle(const std::string& name) const;



			const MeshGPU* getMeshGPU(ResourceHandle handle) const;
			const MeshCPU* getMeshCPU(ResourceHandle handle) const;

			unsigned int getTextureID(ResourceHandle handle) const;

			bool loadMeshes(const std::vector<Scene::Model*>& models);
			bool loadTextures(const std::vector<Scene::TextureData*>& textures);

			bool processTextureConnections(Scene::Scene& scene) const;
			bool processPrimitives(Scene::SceneManager& sm);
			bool processGrids(Scene::SceneManager& sm);

		private:
			uint32_t nextMeshId = 1;
			std::unordered_map<uint32_t, std::string> meshHandleToPath;
			std::unordered_map<std::string, ResourceHandle> meshPathToHandle;

			uint32_t nextTextureId = 1;
			std::unordered_map<uint32_t, std::string> textureHandleToName;
			std::unordered_map<std::string, ResourceHandle> textureNameToHandle;



			MeshManager meshManager;
			MeshFactory meshFactory;
			TextureManager textureManager;
		};
	}
}