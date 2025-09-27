#pragma once

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

struct LightUL {
	int position_UL{ -1 };
	int diffuse_UL{ -1 };
	int attenuation_UL{ -1 };
	int direction_UL{ -1 };
	int param1_UL{ -1 };
	int param2_UL{ -1 };
};

constexpr int SKYBOX_TU{ 20 };

class UniformCache {
public:
	void setProgram(unsigned int programID) { program = programID; }

	bool cacheAllLocations();

	const ModelUL& getModelUL() const { return modelUL; }
	const LightUL& getLightUL() const { return lightUL; }
	int getEyeLocation() const { return eyeLocation; }
	int getSkyboxTextureLocation() const { return skyboxTextureLocation; }
	int getLightCountLocation() const { return lightCountLocation; }
	int getAmbientLightLocation() const { return ambientLightLocation; }

private:
	unsigned int program{ 0 };
	int eyeLocation{ -1 };
	int skyboxTextureLocation{ -1 };
	int lightCountLocation{ -1 }, ambientLightLocation{ -1 };

	ModelUL modelUL;
	LightUL lightUL;

	bool getUniformLocation(int& location, const char* name) const;
	bool cacheTextureUniforms();
	bool cacheCameraUniforms();
	bool cacheModelUniforms();
	bool cacheLightUniforms();
};