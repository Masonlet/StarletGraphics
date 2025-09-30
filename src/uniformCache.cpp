#include "StarletGraphics/uniformCache.hpp"
#include "StarletParser/utils/log.hpp"

#include <glad/glad.h>


bool Cache::getUniformLocation(int& location, const char* name) const {
	location = glGetUniformLocation(program, name);
	if (location < 0) return error("UniformCache", "getUniformLocation", std::string("Could not find uniform: ") + name);
	return true;
}

bool UniformCache::setProgram(unsigned int programID) {
	if (programID == 0) return error("UniformCache", "setProgram", "Program ID is 0");

	program = programID;
	modelCache.setProgram(programID);
	lightCache.setProgram(programID);
	cameraCache.setProgram(programID);
	return true;
}
bool UniformCache::cacheAllLocations() {
	if (program == 0) return error("UniformCache", "cacheAllLocations", "Program ID is 0");

	bool ok = true;
	ok &= cameraCache.cacheLocations();
	ok &= modelCache.cacheLocations();
	ok &= lightCache.cacheLocations();
	return ok;
}



bool CameraCache::cacheLocations() {
	return getUniformLocation(eyeLocation, "eyePos");
}

bool ModelCache::cacheLocations() {
	glUniform1i(glGetUniformLocation(program, "textSampler2D_00"), 0);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_01"), 1);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_02"), 2);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_03"), 3);

	bool ok = true;
	ok &= getUniformLocation(uniform.isSkybox, "bIsSkybox");
	ok &= getUniformLocation(uniform.model, "mModel");
	ok &= getUniformLocation(uniform.modelView, "mView");
	ok &= getUniformLocation(uniform.modelProj, "mProj");
	ok &= getUniformLocation(uniform.modelInverseTranspose, "mModel_InverseTranspose");
	ok &= getUniformLocation(uniform.colourMode, "colourMode");
	ok &= getUniformLocation(uniform.hasVertexColour, "hasVertexColour");
	ok &= getUniformLocation(uniform.colourOverride, "colourOverride");
	ok &= getUniformLocation(uniform.specular, "vertSpecular");
	ok &= getUniformLocation(uniform.yMinMax, "yMin_yMax");
	ok &= getUniformLocation(uniform.seed, "seed");
	ok &= getUniformLocation(uniform.isLit, "bIsLit");
	ok &= getUniformLocation(uniform.useTextures, "bUseTextures");
	ok &= getUniformLocation(uniform.texMixRatios, "texMixRatios");
	ok &= getUniformLocation(skyboxTextureLocation, "skyboxCubeTexture");
	if (skyboxTextureLocation != -1) glUniform1i(skyboxTextureLocation, SKYBOX_TU);
	return ok;
}

bool LightCache::cacheLocations() {
	bool ok = true;
	ok &= getUniformLocation(lightCountLocation, "lightCount");
	ok &= getUniformLocation(ambientLightLocation, "ambientLight");
	ok &= getUniformLocation(uniform.position_UL, "theLights[0].position");
	ok &= getUniformLocation(uniform.diffuse_UL, "theLights[0].diffuse");
	ok &= getUniformLocation(uniform.attenuation_UL, "theLights[0].attenuation");
	ok &= getUniformLocation(uniform.direction_UL, "theLights[0].direction");
	ok &= getUniformLocation(uniform.param1_UL, "theLights[0].param1");
	ok &= getUniformLocation(uniform.param2_UL, "theLights[0].param2");
	return ok;
}