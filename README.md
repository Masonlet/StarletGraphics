# StarletGraphics
A lightweight graphics loading &amp; management library for Starlet projects with Vulkan and OpenGL engines in mind.

## Features

- **Meshes** : 
    - `parsePlyMesh` :ASCII PLY loader (triangles only), reads pos and optional norm/col/texcoords
    - `MeshLoader` : `loadMesh`, `uploadMesh`, `unloadMesh`
    - `MeshManager` : `addMesh`, `createTriangle`, `createSquare`, `createCube`, `findMesh`, `getMesh`

- **Textures**
    - `parseBMP` : Uncompressed 24-bpp BMP reader
    - `TextureLoader` : `loadTexture2D`, `loadCubeFaces`, `uploadTexture2D`, `uploadTextureCube`, `unloadTexture`
    - `TextureManager` : `addTexture`, `addCubeTexture`, `findTexture`, `getTexture`, `getTextureID`

- **Shaders**
    - `ShaderLoader` : `createProgramFromPaths`, `unloadShader`
    - `ShaderManager` : `createProgramFromPaths`, `useProgram`, `getProgramID`

## Folder Conventions (recommended)
- Meshes : `assets/models/*.ply`
- Textures : `assets/textures/*.bmp`
- Shaders : `assets/shaders/*.glsl`
**Managers** support a `basePath` you can set to your `assets` folders.

## Using as a Dependency

```cmake
include(FetchContent)

FetchContent_Declare(StarletGraphics GIT_REPOSITORY https://github.com/Masonlet/StarletGraphics.git GIT_TAG main)
FetchContent_MakeAvailable(StarletGraphics)

target_link_libraries(YourAppName PRIVATE StarletGraphics)
```
