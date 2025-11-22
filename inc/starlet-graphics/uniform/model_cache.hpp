#pragma once

#include "starlet-graphics/uniform/cache.hpp"

namespace Starlet::Graphics {
	struct ModelUL {
		int model{ -1 };
		int modelInverseTranspose{ -1 };
		int modelView{ -1 }, modelProj{ -1 };
		int isSkybox{ -1 };

		int colourMode{ -1 };
		int colourOverride{ -1 };
		int hasVertexColour{ -1 };
		int specular{ -1 };

		int yMinMax{ -1 };
		int seed{ -1 };

		int isLit{ -1 };

		int useTextures{ -1 };
		int texMixRatios{ -1 };
	};

	constexpr int SKYBOX_TU{ 20 };

	class ModelCache : public Cache {
	public:
		bool cacheLocations() override;
		int getSkyboxTextureLocation() const { return skyboxTextureLocation; }
		const ModelUL& getModelUL() const { return uniform; }

	private:
		ModelUL uniform;
		int skyboxTextureLocation{ -1 };
	};
}