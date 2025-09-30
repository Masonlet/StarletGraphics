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

class Cache {
public :
	void setProgram(unsigned int programID) { program = programID; }	
	virtual bool cacheLocations() = 0;
	virtual ~Cache() = default;

protected:
	unsigned int program{ 0 };
	bool getUniformLocation(int& location, const char* name) const;
};

class LightCache : public Cache {
public:
	bool cacheLocations() override;
	int getLightCountLocation() const { return lightCountLocation; }
	int getAmbientLightLocation() const { return ambientLightLocation; }
	const LightUL& getLightUL() const { return uniform; }

private:
	int lightCountLocation{ -1 };
	int ambientLightLocation{ -1 };
	LightUL uniform;
};

class ModelCache : public Cache {
public:
	bool cacheLocations() override;
	int getSkyboxTextureLocation() const { return skyboxTextureLocation; }
	const ModelUL& getModelUL() const { return uniform; }

private:
	ModelUL uniform;
	int skyboxTextureLocation{ -1 };
};

class CameraCache : public Cache {
public:
	bool cacheLocations() override;
	int getEyeLocation() const { return eyeLocation; }

private:
	int eyeLocation{ -1 };
};

class UniformCache {
public:
	bool setProgram(unsigned int programID);
	bool cacheAllLocations();

	const ModelCache& getModelCache() const { return modelCache; }
	const LightCache& getLightCache() const { return lightCache; }
	const CameraCache& getCameraCache() const { return cameraCache; }

private:
	unsigned int program{ 0 };
	ModelCache modelCache;
	LightCache lightCache;
	CameraCache cameraCache;
};