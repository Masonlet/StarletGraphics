#include "StarletGraphics/loader/resourceLoader.hpp"
#include "StarletGraphics/manager/resourceManager.hpp"
#include "StarletSerializer/utils/log.hpp"

#include "StarletScene/sceneManager.hpp"
#include "StarletScene/components/model.hpp"
#include "StarletScene/components/light.hpp"
#include "StarletScene/components/camera.hpp"
#include "StarletScene/components/grid.hpp"
#include "StarletScene/components/textureData.hpp"
#include "StarletScene/components/primitive.hpp"
#include "StarletScene/components/transform.hpp"
#include "StarletScene/components/colour.hpp"

bool ResourceLoader::loadMeshes(const std::vector<Model*>& models) {
  for (Model* model : models) {
    if (!resourceManager.getMeshManager().loadAndAddMesh(model->meshPath))
      return error("ResourceLoader", "loadMeshes", "Failed to load/add mesh: " + model->meshPath);

    model->meshHandle = resourceManager.addMesh(model->meshPath);

    if (!model->meshHandle.isValid())
      return error("ResourceLoader", "loadMeshes", "Failed to register mesh: " + model->meshPath);
  }

  return debugLog("ResourceLoader", "loadMeshes", "Loaded and registered " + std::to_string(models.size()) + " meshes");
}
bool ResourceLoader::loadTextures(const std::vector<TextureData*>& textures) {
  for (const TextureData* texture : textures) {
    unsigned int textureID = 0;

    if (!texture->isCube) {
      if (!resourceManager.getTextureManager().addTexture(texture->name, texture->faces[0]))
        return error("ResourceLoader", "loadTextures", "Failed to load 2D texture: " + texture->name);

      textureID = resourceManager.getTextureManager().getTextureID(texture->name);
    }
    else {
      if (!resourceManager.getTextureManager().addTextureCube(texture->name, texture->faces))
        return error("ResourceLoader", "loadTextures", "Failed to load cube map: " + texture->name);

      textureID = resourceManager.getTextureManager().getTextureID(texture->name);
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

    Model* model = sm.getScene().addComponent<Model>(entity);
    if (!model) return error("Engine", "loadScenePrimitives", "Failed to create model component for primitive: " + primitive->name);

    model->name = primitive->name;
    model->meshPath = primitive->name;
    model->meshHandle = resourceManager.addMesh(primitive->name);
    model->useTextures = false;

    for (unsigned i = 0; i < Model::NUM_TEXTURES; ++i) {
      model->textureNames[i].clear();
      model->textureHandles[i] = TextureHandle{ 0 };
      model->textureMixRatio[i] = 0.0f;
    }
  }

  return true;
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

    MeshHandle sharedMeshHandle = resourceManager.addMesh(sharedName);

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

bool ResourceLoader::createTriangle(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour) {
  MeshCPU info;

  info.numVertices = 3;
  info.vertices.resize(3);
  info.vertices[0].pos = { -0.5f * size.x, -0.5f * size.y, 0.0f };
  info.vertices[1].pos = { 0.5f * size.x, -0.5f * size.y, 0.0f };
  info.vertices[2].pos = { 0.0f,           0.5f * size.y, 0.0f };
  for (int i = 0; i < 3; ++i)	info.vertices[i].col = vertexColour;

  info.numIndices = 3;
  info.numTriangles = 1;
  info.indices.resize(3);
  info.indices[0] = 0;
  info.indices[1] = 1;
  info.indices[2] = 2;

  return resourceManager.getMeshManager().addMesh(name, info)
    ? debugLog("Primitive", "createTriangle", "Added triangle: " + name)
    : error("Primitive", "createTriangle", "Failed to create triangle " + name);
}

bool ResourceLoader::createSquare(const std::string& name, const Vec2<float>& size, const Vec4<float>& vertexColour) {
  MeshCPU info;
  info.numVertices = 6;
  info.numIndices = 6;
  info.numTriangles = 2;

  info.vertices.resize(6);
  info.indices.resize(6);

  const float halfX = 0.5f * size.x;
  const float halfY = 0.5f * size.y;

  // First triangle
  info.vertices[0].pos = { -halfX, -halfY, 0.0f };
  info.vertices[1].pos = { halfX, -halfY, 0.0f };
  info.vertices[2].pos = { halfX,  halfY, 0.0f };

  // Second triangle
  info.vertices[3].pos = { -halfX, -halfY, 0.0f };
  info.vertices[4].pos = { halfX,  halfY, 0.0f };
  info.vertices[5].pos = { -halfX,  halfY, 0.0f };

  for (int i = 0; i < 6; ++i) {
    info.vertices[i].col = vertexColour;
    info.indices[i] = i;
  }

  return resourceManager.getMeshManager().addMesh(name, info)
    ? debugLog("Primitive", "createSquare", "Added square: " + name)
    : error("Primitive", "createSquare", "Failed to create square " + name);
}

bool ResourceLoader::createCube(const std::string& name, const Vec3<float>& size, const Vec4<float>& vertexColour) {
  MeshCPU info;
  constexpr int vertexCount = 36;

  info.numVertices = vertexCount;
  info.numIndices = vertexCount;
  info.numTriangles = 12;

  info.vertices.resize(vertexCount);
  info.indices.resize(vertexCount);

  float x = 0.5f * size.x;
  float y = 0.5f * size.y;
  float z = 0.5f * size.z;

  Vec3<float> positions[8] = {
    {-x, -y, -z}, { x, -y, -z}, { x,  y, -z}, { -x,  y, -z}, // Back
    {-x, -y,  z}, { x, -y,  z}, { x,  y,  z}, { -x,  y,  z}  // Front
  };

  unsigned int faces[6][6] = {
    {4, 5, 6, 4, 6, 7}, // Front
    {1, 0, 3, 1, 3, 2}, // Back
    {0, 4, 7, 0, 7, 3}, // Left
    {5, 1, 2, 5, 2, 6}, // Right
    {3, 7, 6, 3, 6, 2}, // Top
    {0, 1, 5, 0, 5, 4}  // Bottom
  };

  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < 6; ++j) {
      int idx = i * 6 + j;

      info.vertices[idx].pos = positions[faces[i][j]];
      info.vertices[idx].col = vertexColour;
      info.indices[idx] = idx;
    }
  }

  return resourceManager.getMeshManager().addMesh(name, info)
    ? debugLog("Primitive", "createCube", "Added cube: " + name)
    : error("Primitive", "createCube", "Failed to create cube " + name);
}
bool ResourceLoader::createPrimitiveMesh(const Primitive& primitive, const TransformComponent& transform, const ColourComponent& colour) {
  switch (primitive.type) {
  case PrimitiveType::Triangle:
    return createTriangle(primitive.name, { transform.size.x, transform.size.y }, colour.colour);
  case PrimitiveType::Square:
    return createSquare(primitive.name, { transform.size.x, transform.size.y }, colour.colour);
  case PrimitiveType::Cube:
    return createCube(primitive.name, transform.size, colour.colour);
  default:
    return error("Renderer", "loadScenePrimitives", "Invalid primitive: " + primitive.name);
  }
}

bool ResourceLoader::createGridMesh(const Grid& grid, const std::string& meshName, const TransformComponent& transform, const ColourComponent& colour) {
  switch (grid.type) {
  case GridType::Square:
    return createSquare(meshName, { transform.size.x, transform.size.y }, colour.colour);
  case GridType::Cube:
    return createCube(meshName, transform.size, colour.colour);
  default:
    return error("Renderer", "createGridMesh", "Invalid grid: " + grid.name + ", mesh: " + meshName);
  }
}