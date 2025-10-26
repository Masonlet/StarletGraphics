#pragma once

#include <string>

namespace Starlet {
	namespace Math {
		template<typename T> struct Vec2;
		template<typename T> struct Vec3;
		template<typename T> struct Vec4;
	}

	namespace Scene {
		struct Primitive;
		struct Grid;

		struct TransformComponent;
		struct ColourComponent;
	}

	namespace Graphics {
		class MeshManager;

		class MeshFactory {
		public:
			MeshFactory(MeshManager& mm) : meshManager(mm) {};

			bool createPrimitiveMesh(const Scene::Primitive& primitive, const Scene::TransformComponent& transform, const Scene::ColourComponent& colour);
			bool createGridMesh(const Scene::Grid& grid, const std::string& meshName, const Scene::TransformComponent& transform, const Scene::ColourComponent& colour);

		private:
			bool createTriangle(const std::string& name, const Math::Vec2<float>& size, const Math::Vec4<float>& vertexColour);
			bool createSquare(const std::string& name, const Math::Vec2<float>& size, const Math::Vec4<float>& vertexColour);
			bool createCube(const std::string& name, const Math::Vec3<float>& size, const Math::Vec4<float>& vertexColour);

			MeshManager& meshManager;
		};
	}
}