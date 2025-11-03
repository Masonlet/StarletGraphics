#include "StarletGraphics/manager/resourceManager.hpp"
#include "StarletLogger/logger.hpp"

#include "StarletScene/manager/sceneManager.hpp"
#include "StarletScene/component/primitive.hpp"
#include "StarletScene/component/model.hpp"
#include "StarletScene/component/textureData.hpp"
#include "StarletScene/component/grid.hpp"
#include "StarletScene/component/transform.hpp"
#include "StarletScene/component/colour.hpp"

namespace Starlet::Graphics {
  ResourceManager::ResourceManager() : meshFactory(meshManager) {}

  void ResourceManager::setBasePath(const std::string& path) {
    meshManager.setBasePath((path + "/models/").c_str());
    textureManager.setBasePath((path + "/textures/").c_str());
  }

  ResourceHandle ResourceManager::addMesh(const std::string& path) {
    auto it = meshPathToHandle.find(path);
    if (it != meshPathToHandle.end()) return it->second;

    ResourceHandle handle{ nextMeshId++ };
    meshPathToHandle[path] = handle;
    meshHandleToPath[handle.id] = path;
    return handle;
  }
  bool ResourceManager::hasMesh(const std::string& path) const {
    return meshPathToHandle.find(path) != meshPathToHandle.end();
  }
  bool ResourceManager::hasMesh(ResourceHandle handle) const {
    return handle.isValid() && meshHandleToPath.find(handle.id) != meshHandleToPath.end();
  }
  ResourceHandle ResourceManager::getMeshHandle(const std::string& path) const {
    auto it = meshPathToHandle.find(path);
    return it != meshPathToHandle.end() ? it->second : ResourceHandle{ 0 };
  }

  ResourceHandle ResourceManager::addTexture(const std::string& name, unsigned int textureID) {
    auto it = textureNameToHandle.find(name);
    if (it != textureNameToHandle.end()) return it->second;

    ResourceHandle handle{ nextTextureId++ };
    textureHandleToName[handle.id] = name;
    textureNameToHandle[name] = handle;
    return handle;
  }
  bool ResourceManager::hasTexture(const std::string& name) const {
    return textureNameToHandle.find(name) != textureNameToHandle.end();
  }
  bool ResourceManager::hasTexture(ResourceHandle handle) const {
    return handle.isValid() && textureHandleToName.find(handle.id) != textureHandleToName.end();
  }
  ResourceHandle ResourceManager::getTextureHandle(const std::string& name) const {
    auto it = textureNameToHandle.find(name);
    return it != textureNameToHandle.end() ? it->second : ResourceHandle{ 0 };
  }



  unsigned int ResourceManager::getTextureID(ResourceHandle handle) const {
    if (!handle.isValid()) return 0;

    auto it = textureHandleToName.find(handle.id);
    if (it == textureHandleToName.end()) return 0;

    return textureManager.getTextureID(it->second);
  }

  const MeshGPU* ResourceManager::getMeshGPU(ResourceHandle handle) const {
    if (!handle.isValid()) return nullptr;

    auto it = meshHandleToPath.find(handle.id);
    if (it == meshHandleToPath.end()) return nullptr;

    return meshManager.getMeshGPU(it->second);
  }
  const MeshCPU* ResourceManager::getMeshCPU(ResourceHandle handle) const {
    if (!handle.isValid()) return nullptr;

    auto it = meshHandleToPath.find(handle.id);
    if (it == meshHandleToPath.end()) return nullptr;

    return meshManager.getMeshCPU(it->second);
  }


  bool ResourceManager::loadMeshes(const std::vector<Scene::Model*>& models) {
    for (Scene::Model* model : models) {
      if (!meshManager.loadAndAddMesh(model->meshPath))
        return Logger::error("ResourceLoader", "loadMeshes", "Failed to load/add mesh: " + model->meshPath);

      model->meshHandle = addMesh(model->meshPath);

      if (!model->meshHandle.isValid())
        return Logger::error("ResourceLoader", "loadMeshes", "Failed to register mesh: " + model->meshPath);
    }

    return Logger::debugLog("ResourceLoader", "loadMeshes", "Loaded and registered " + std::to_string(models.size()) + " meshes");
  }
  bool ResourceManager::loadTextures(const std::vector<Scene::TextureData*>& textures) {
    for (const Scene::TextureData* texture : textures) {
      unsigned int textureID = 0;

      if (!texture->isCube) {
        if (!textureManager.addTexture(texture->name, texture->faces[0]))
          return Logger::error("ResourceLoader", "loadTextures", "Failed to load 2D texture: " + texture->name);

        textureID = textureManager.getTextureID(texture->name);
      }
      else {
        if (!textureManager.addTextureCube(texture->name, texture->faces))
          return Logger::error("ResourceLoader", "loadTextures", "Failed to load cube map: " + texture->name);

        textureID = textureManager.getTextureID(texture->name);
      }

      if (textureID == 0) return Logger::error("ResourceLoader", "loadTextures", "Failed to get texture ID for: " + texture->name);

      ResourceHandle handle = addTexture(texture->name, textureID);
      if (!handle.isValid())
        return Logger::error("ResourceLoader", "loadTextures", "Failed to add texture: " + texture->name);
    }

    return Logger::debugLog("ResourceLoader", "loadTextures", "Loaded and added " + std::to_string(textures.size()) + " textures");
  }

  bool ResourceManager::processTextureConnections(Scene::Scene& scene) const {
    for (Scene::Model* model : scene.getComponentsOfType<Scene::Model>()) {
      if (model->name == "skybox") {
        ResourceHandle handle = getTextureHandle("skybox");
        if (!handle.isValid()) return Logger::error("ResourceLoader", "processTextureConnection", "Failed to get skybox texture handle");

        model->textureHandles[0] = handle;
        continue;
      }

      if (!model->useTextures) continue;

      for (size_t i = 0; i < Scene::Model::NUM_TEXTURES; ++i) {
        if (model->textureNames[i].empty()) continue;
        model->textureHandles[i] = getTextureHandle(model->textureNames[i]);

        if (!model->textureHandles[i].isValid())
          return Logger::error("ResourceLoader", "processTextureConnection", "Invalid texture handle for: " + model->textureNames[i]);
      }
    }

    return true;
  }

  bool ResourceManager::processPrimitives(Scene::SceneManager& sm) {
    for (Scene::Primitive* primitive : sm.getScene().getComponentsOfType<Scene::Primitive>()) {
      const Scene::Entity entity = primitive->id;
      if (!sm.getScene().hasComponent<Scene::TransformComponent>(entity))
        return Logger::error("Engine", "processPrimitives", "Primitive entity has no transform component.");

      const Scene::TransformComponent& transform = sm.getScene().getComponent<Scene::TransformComponent>(entity);

      const Scene::ColourComponent* colour = nullptr;
      if (sm.getScene().hasComponent<Scene::ColourComponent>(entity)) {
        colour = &sm.getScene().getComponent<Scene::ColourComponent>(entity);
      }

      const Scene::ColourComponent defaultColour{};
      if (!meshFactory.createPrimitiveMesh(*primitive, transform, colour ? *colour : defaultColour))
        return Logger::error("ResourceLoader", "processPrimitives", "Failed to create mesh for primitive: " + primitive->name);

      Scene::Model* model = sm.getScene().addComponent<Scene::Model>(entity);
      if (!model) return Logger::error("Engine", "processPrimitives", "Failed to create model component for primitive: " + primitive->name);

      model->name = primitive->name;
      model->meshPath = primitive->name;
      model->meshHandle = addMesh(primitive->name);
      model->useTextures = false;

      for (unsigned i = 0; i < Scene::Model::NUM_TEXTURES; ++i) {
        model->textureNames[i].clear();
        model->textureHandles[i] = ResourceHandle{ 0 };
        model->textureMixRatio[i] = 0.0f;
      }
    }

    return true;
  }

  bool ResourceManager::processGrids(Scene::SceneManager& sceneManager) {
    for (const Scene::Grid* grid : sceneManager.getScene().getComponentsOfType<Scene::Grid>()) {
      std::string sharedName = grid->name + (grid->type == Scene::GridType::Square ? "_sharedSquare" : "_sharedCube");

      const Scene::Entity entity = grid->id;
      if (!sceneManager.getScene().hasComponent<Scene::TransformComponent>(entity))
        return Logger::error("Engine", "processGrids", "Grid entity has no transform component.");

      const Scene::TransformComponent& gridTransform = sceneManager.getScene().getComponent<Scene::TransformComponent>(entity);

      const Scene::ColourComponent* colour = nullptr;
      if (sceneManager.getScene().hasComponent<Scene::ColourComponent>(entity)) {
        colour = &sceneManager.getScene().getComponent<Scene::ColourComponent>(entity);
      }

      const Scene::ColourComponent defaultColour{};
      if (!meshFactory.createGridMesh(*grid, sharedName, gridTransform, colour ? *colour : defaultColour))
        return Logger::error("ResourceLoader", "processGrids", "Failed to create mesh for: " + sharedName);

      ResourceHandle sharedMeshHandle = addMesh(sharedName);

      const int gridSide = (grid->count > 0) ? static_cast<int>(std::ceil(std::sqrt(static_cast<float>(grid->count)))) : 0;
      for (int i = 0; i < 0 + grid->count; ++i) {
        const int row = (gridSide > 0) ? (i / gridSide) : 0;
        const int col = (gridSide > 0) ? (i % gridSide) : 0;

        Math::Vec3<float> pos{};
        if (grid->type == Scene::GridType::Square) {
          pos = { grid->spacing * static_cast<float>(col),
                  grid->spacing * static_cast<float>(row),
                  0.0f };
        }
        else {
          pos = { grid->spacing * static_cast<float>(col),
                  0.0f,
                  grid->spacing * static_cast<float>(row) };
        }

        Scene::Entity e = sceneManager.getScene().createEntity();

        Scene::TransformComponent* transform = sceneManager.getScene().addComponent<Scene::TransformComponent>(e);
        if (!transform) return Logger::error("ResourceLoader", "processGrids", "Failed to add TransformComponent");
        transform->pos = pos;

        if (colour) {
          Scene::ColourComponent* instanceColour = sceneManager.getScene().addComponent<Scene::ColourComponent>(e);
          if (!instanceColour) return Logger::error("ResourceLoader", "processGrids", "Failed to add ColourComponent");
          *instanceColour = *colour;
        }

        Scene::Model* model = sceneManager.getScene().addComponent<Scene::Model>(e);
        if (!model) return Logger::error("Engine", "processGrids", "Failed to add grid instance model: " + sharedName);

        model->name = grid->name + "_instance_" + std::to_string(i);
        model->meshPath = sharedName;
        model->meshHandle = sharedMeshHandle;
        model->useTextures = false;

        for (unsigned ti = 0; ti < Scene::Model::NUM_TEXTURES; ++ti) {
          model->textureNames[ti].clear();
          model->textureHandles[ti] = ResourceHandle{ 0 };
          model->textureMixRatio[ti] = 0.0f;
        }
      }
    }

    return true;
  }
}