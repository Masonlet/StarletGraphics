#include "StarletGraphics/loader/resourceLoader.hpp"
#include "StarletGraphics/manager/resourceManager.hpp"
#include "StarletParser/utils/log.hpp"

#include "StarletScene/sceneManager.hpp"
#include "StarletScene/components/model.hpp"
#include "StarletScene/components/light.hpp"
#include "StarletScene/components/camera.hpp"
#include "StarletScene/components/grid.hpp"
#include "StarletScene/components/textureData.hpp"
#include "StarletScene/components/primitive.hpp"
#include "StarletScene/components/transform.hpp"
#include "StarletScene/components/colour.hpp"

void ResourceLoader::setBasePath(const std::string& path) { 
  basePath = path; 
  meshManager.setBasePath((path + "/models/").c_str());
  textureManager.setBasePath((path + "/textures/").c_str());
}

bool ResourceLoader::loadMeshes(const std::vector<Model*>& models) {
  for (Model* model : models) {
    if (!meshManager.loadAndAddMesh(model->meshPath))
      return error("ResourceLoader", "loadMeshes", "Failed to load/add mesh: " + model->meshPath);

    MeshGPU* gpuMesh;
    if (!meshManager.getMeshGPU(model->meshPath, gpuMesh))
      return error("ResourceLoader", "loadMeshes", "Failed to get GPU mesh: " + model->meshPath);

    const MeshCPU* cpuMesh;
    if (!meshManager.getMeshCPU(model->meshPath, cpuMesh))
      return error("ResourceLoader", "loadMeshes", "Failed to get CPU mesh: " + model->meshPath);

    model->meshHandle = resourceManager.addMesh(model->meshPath, gpuMesh, cpuMesh);

    if (!model->meshHandle.isValid())
      return error("ResourceLoader", "loadMeshes", "Failed to register mesh: " + model->meshPath);
  }

  return debugLog("ResourceLoader", "loadMeshes", "Loaded and registered " + std::to_string(models.size()) + " meshes");
}
bool ResourceLoader::loadTextures(const std::vector<TextureData*>& textures) {
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

    TextureHandle handle = resourceManager.addTexture(texture->name, textureID);
    if (!handle.isValid())
      return error("ResourceLoader", "loadTextures", "Failed to add texture: " + texture->name);
  }

  return debugLog("ResourceLoader", "loadTextures", "Loaded and added " + std::to_string(textures.size()) + " textures");
}

bool ResourceLoader::processTextureConnections(Scene& scene) {
  for (Model* model : scene.getComponentsOfType<Model>()) {
    if (model->name == "skybox") {
      TextureHandle handle = resourceManager.getTextureHandle("skybox");
      if (!handle.isValid()) return error("ResourceLoader", "processTextureConnection", "Failed to get skybox texture handle");

      model->textureHandles[0] = handle;
      continue;
    }

    if (!model->useTextures) continue;

    for (size_t i = 0; i < Model::NUM_TEXTURES; ++i) {
      if (model->textureNames[i].empty()) continue;
      model->textureHandles[i] = resourceManager.getTextureHandle(model->textureNames[i]);

      if (!model->textureHandles[i].isValid()) 
        return error("ResourceLoader", "processTextureConnection", "Invalid texture handle for: " + model->textureNames[i]);
    }
  }

  return true;
}

bool ResourceLoader::createPrimitiveMesh(const Primitive& primitive, const TransformComponent& transform, const ColourComponent& colour) {
  switch (primitive.type) {
  case PrimitiveType::Triangle:
    return meshManager.createTriangle(primitive.name, { transform.size.x, transform.size.y }, colour.colour);
  case PrimitiveType::Square:
    return meshManager.createSquare(primitive.name, { transform.size.x, transform.size.y }, colour.colour);
  case PrimitiveType::Cube:
    return meshManager.createCube(primitive.name, transform.size, colour.colour);
  default:
    return error("Renderer", "loadScenePrimitives", "Invalid primitive: " + primitive.name);
  }
}
bool ResourceLoader::processPrimitives(SceneManager& sm) {
  for (Primitive* primitive : sm.getScene().getComponentsOfType<Primitive>()) {
    const StarEntity entity = primitive->id;
    if (!sm.getScene().hasComponent<TransformComponent>(entity))
      return error("Engine", "loadScenePrimitives", "Primitive entity has no transform component.");

    const TransformComponent& transform = sm.getScene().getComponent<TransformComponent>(entity);

    const ColourComponent* colour = nullptr;
    if (sm.getScene().hasComponent<ColourComponent>(entity)) {
      colour = &sm.getScene().getComponent<ColourComponent>(entity);
    }

    const ColourComponent defaultColour{};
    if (!createPrimitiveMesh(*primitive, transform, colour ? *colour : defaultColour))
      return error("ResourceLoader", "processPrimitives", "Failed to create mesh for primitive: " + primitive->name);

    MeshGPU* primMesh;
    if (!meshManager.getMeshGPU(primitive->name, primMesh))
      return error("ResourceLoader", "processPrimitives", "Failed to get GPU mesh: " + primitive->name);

    const MeshCPU* cpuMesh;
    if (!meshManager.getMeshCPU(primitive->name, cpuMesh))
      return error("ResourceLoader", "processPrimitives", "Failed to get CPU mesh: " + primitive->name);

    Model* model = sm.getScene().addComponent<Model>(entity);
    if (!model) return error("Engine", "loadScenePrimitives", "Failed to create model component for primitive: " + primitive->name);

    model->name = primitive->name;
    model->meshPath = primitive->name;
    model->meshHandle = resourceManager.addMesh(primitive->name, primMesh, cpuMesh);
    model->useTextures = false;

    for (unsigned i = 0; i < Model::NUM_TEXTURES; ++i) {
      model->textureNames[i].clear();
      model->textureHandles[i] = TextureHandle{ 0 };
      model->textureMixRatio[i] = 0.0f;
    }
  }

  return true;
}

bool ResourceLoader::createGridMesh(const Grid& grid, const std::string& meshName, const TransformComponent& transform, const ColourComponent& colour) {
  switch (grid.type) {
  case GridType::Square:
    return meshManager.createSquare(meshName, { transform.size.x, transform.size.y }, colour.colour);
  case GridType::Cube:
    return meshManager.createCube(meshName, transform.size, colour.colour);
  default:
    return error("Renderer", "createGridMesh", "Invalid grid: " + grid.name + ", mesh: " + meshName);
  }
}
bool ResourceLoader::processGrids(SceneManager& sceneManager) {
  for (const Grid* grid : sceneManager.getScene().getComponentsOfType<Grid>()) {
    std::string sharedName = grid->name + (grid->type == GridType::Square ? "_sharedSquare" : "_sharedCube");

    const StarEntity entity = grid->id;
    if (!sceneManager.getScene().hasComponent<TransformComponent>(entity))
      return error("Engine", "loadSceneGrids", "Grid entity has no transform component.");

    const TransformComponent& gridTransform = sceneManager.getScene().getComponent<TransformComponent>(entity);

    const ColourComponent* colour = nullptr;
    if (sceneManager.getScene().hasComponent<ColourComponent>(entity)) {
      colour = &sceneManager.getScene().getComponent<ColourComponent>(entity);
    }

    const ColourComponent defaultColour{};
    if (!createGridMesh(*grid, sharedName, gridTransform, colour ? *colour : defaultColour))
      return error("ResourceLoader", "processGrids", "Failed to create mesh for: " + sharedName);

    MeshGPU* gpuMesh;
    if (!meshManager.getMeshGPU(sharedName, gpuMesh))
      return error("ResourceLoader", "processGrids", "Failed to get GPU mesh: " + sharedName);

    const MeshCPU* cpuMesh;
    if (!meshManager.getMeshCPU(sharedName, cpuMesh))
      return error("ResourceLoader", "processGrids", "Failed to get CPU mesh: " + sharedName);

    MeshHandle sharedMeshHandle = resourceManager.addMesh(sharedName, gpuMesh, cpuMesh);

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

      Model* model = sceneManager.getScene().addComponent<Model>(e);
      if (!model) return error("Engine", "loadSceneGrids", "Failed to add grid instance model: " + sharedName);

      model->name = grid->name + "_instance_" + std::to_string(i);
      model->meshPath = sharedName;
      model->meshHandle = sharedMeshHandle;  
      model->useTextures = false;

      for (unsigned ti = 0; ti < Model::NUM_TEXTURES; ++ti) {
        model->textureNames[ti].clear();
        model->textureHandles[ti] = TextureHandle{ 0 };
        model->textureMixRatio[ti] = 0.0f;
      }
    }
  }

  return true;
}