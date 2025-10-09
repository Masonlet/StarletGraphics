#include "StarletGraphics/manager/resourceManager.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletScene/sceneManager.hpp"
#include "StarletScene/components/primitive.hpp"
#include "StarletScene/components/model.hpp"
#include "StarletScene/components/textureData.hpp"
#include "StarletScene/components/grid.hpp"
#include "StarletScene/components/transform.hpp"
#include "StarletScene/components/colour.hpp"

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


bool ResourceManager::loadMeshes(const std::vector<Model*>& models) {
  for (Model* model : models) {
    if (!meshManager.loadAndAddMesh(model->meshPath))
      return error("ResourceLoader", "loadMeshes", "Failed to load/add mesh: " + model->meshPath);

    model->meshHandle = addMesh(model->meshPath);

    if (!model->meshHandle.isValid())
      return error("ResourceLoader", "loadMeshes", "Failed to register mesh: " + model->meshPath);
  }

  return debugLog("ResourceLoader", "loadMeshes", "Loaded and registered " + std::to_string(models.size()) + " meshes");
}
bool ResourceManager::loadTextures(const std::vector<TextureData*>& textures) {
  for (const TextureData* texture : textures) {
    unsigned int textureID = 0;

    if (!texture->isCube) {
      if (!textureManager.addTexture(texture->name, texture->faces[0]))
        return error("ResourceLoader", "loadTextures", "Failed to load 2D texture: " + texture->name);

      textureID = textureManager.getTextureID(texture->name);
    }
    else {
      if (!textureManager.addTextureCube(texture->name, texture->faces))
        return error("ResourceLoader", "loadTextures", "Failed to load cube map: " + texture->name);

      textureID = textureManager.getTextureID(texture->name);
    }

    if (textureID == 0) return error("ResourceLoader", "loadTextures", "Failed to get texture ID for: " + texture->name);

    ResourceHandle handle = addTexture(texture->name, textureID);
    if (!handle.isValid())
      return error("ResourceLoader", "loadTextures", "Failed to add texture: " + texture->name);
  }

  return debugLog("ResourceLoader", "loadTextures", "Loaded and added " + std::to_string(textures.size()) + " textures");
}

bool ResourceManager::processTextureConnections(Scene& scene) {
  for (Model* model : scene.getComponentsOfType<Model>()) {
    if (model->name == "skybox") {
      ResourceHandle handle = getTextureHandle("skybox");
      if (!handle.isValid()) return error("ResourceLoader", "processTextureConnection", "Failed to get skybox texture handle");

      model->textureHandles[0] = handle;
      continue;
    }

    if (!model->useTextures) continue;

    for (size_t i = 0; i < Model::NUM_TEXTURES; ++i) {
      if (model->textureNames[i].empty()) continue;
      model->textureHandles[i] = getTextureHandle(model->textureNames[i]);

      if (!model->textureHandles[i].isValid())
        return error("ResourceLoader", "processTextureConnection", "Invalid texture handle for: " + model->textureNames[i]);
    }
  }

  return true;
}

bool ResourceManager::processPrimitives(SceneManager& sm) {
  for (Primitive* primitive : sm.getScene().getComponentsOfType<Primitive>()) {
    const StarEntity entity = primitive->id;
    if (!sm.getScene().hasComponent<TransformComponent>(entity))
      return error("Engine", "processPrimitives", "Primitive entity has no transform component.");

    const TransformComponent& transform = sm.getScene().getComponent<TransformComponent>(entity);

    const ColourComponent* colour = nullptr;
    if (sm.getScene().hasComponent<ColourComponent>(entity)) {
      colour = &sm.getScene().getComponent<ColourComponent>(entity);
    }

    const ColourComponent defaultColour{};
    if (!meshFactory.createPrimitiveMesh(*primitive, transform, colour ? *colour : defaultColour))
      return error("ResourceLoader", "processPrimitives", "Failed to create mesh for primitive: " + primitive->name);

    Model* model = sm.getScene().addComponent<Model>(entity);
    if (!model) return error("Engine", "processPrimitives", "Failed to create model component for primitive: " + primitive->name);

    model->name = primitive->name;
    model->meshPath = primitive->name;
    model->meshHandle = addMesh(primitive->name);
    model->useTextures = false;

    for (unsigned i = 0; i < Model::NUM_TEXTURES; ++i) {
      model->textureNames[i].clear();
      model->textureHandles[i] = ResourceHandle{ 0 };
      model->textureMixRatio[i] = 0.0f;
    }
  }

  return true;
}

bool ResourceManager::processGrids(SceneManager& sceneManager) {
  for (const Grid* grid : sceneManager.getScene().getComponentsOfType<Grid>()) {
    std::string sharedName = grid->name + (grid->type == GridType::Square ? "_sharedSquare" : "_sharedCube");

    const StarEntity entity = grid->id;
    if (!sceneManager.getScene().hasComponent<TransformComponent>(entity))
      return error("Engine", "processGrids", "Grid entity has no transform component.");

    const TransformComponent& gridTransform = sceneManager.getScene().getComponent<TransformComponent>(entity);

    const ColourComponent* colour = nullptr;
    if (sceneManager.getScene().hasComponent<ColourComponent>(entity)) {
      colour = &sceneManager.getScene().getComponent<ColourComponent>(entity);
    }

    const ColourComponent defaultColour{};
    if (!meshFactory.createGridMesh(*grid, sharedName, gridTransform, colour ? *colour : defaultColour))
      return error("ResourceLoader", "processGrids", "Failed to create mesh for: " + sharedName);

    ResourceHandle sharedMeshHandle = addMesh(sharedName);

    const int gridSide = (grid->count > 0) ? static_cast<int>(std::ceil(std::sqrt(static_cast<float>(grid->count)))) : 0;
    for (int i = 0; i < 0 + grid->count; ++i) {
      const int row = (gridSide > 0) ? (i / gridSide) : 0;
      const int col = (gridSide > 0) ? (i % gridSide) : 0;

      Vec3<float> pos{};
      if (grid->type == GridType::Square) {
        pos = { grid->spacing * static_cast<float>(col),
                grid->spacing * static_cast<float>(row),
                0.0f };
      }
      else {
        pos = { grid->spacing * static_cast<float>(col),
                0.0f,
                grid->spacing * static_cast<float>(row) };
      }

      StarEntity e = sceneManager.getScene().createEntity();

      TransformComponent* transform = sceneManager.getScene().addComponent<TransformComponent>(e);
      if (!transform) return error("ResourceLoader", "processGrids", "Failed to add TransformComponent");
      transform->pos = pos;

      if (colour) {
        ColourComponent* instanceColour = sceneManager.getScene().addComponent<ColourComponent>(e);
        if (!instanceColour) return error("ResourceLoader", "processGrids", "Failed to add ColourComponent");
        *instanceColour = *colour;
      }

      Model* model = sceneManager.getScene().addComponent<Model>(e);
      if (!model) return error("Engine", "processGrids", "Failed to add grid instance model: " + sharedName);

      model->name = grid->name + "_instance_" + std::to_string(i);
      model->meshPath = sharedName;
      model->meshHandle = sharedMeshHandle;
      model->useTextures = false;

      for (unsigned ti = 0; ti < Model::NUM_TEXTURES; ++ti) {
        model->textureNames[ti].clear();
        model->textureHandles[ti] = ResourceHandle{ 0 };
        model->textureMixRatio[ti] = 0.0f;
      }
    }
  }

  return true;
}