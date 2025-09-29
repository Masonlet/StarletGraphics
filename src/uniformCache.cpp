#include "StarletGraphics/uniformCache.hpp"
#include "StarletParser/utils/log.hpp"

#include <glad/glad.h>

bool UniformCache::cacheAllLocations() {
	if (program == 0) return error("UniformCache", "cacheAllLocations", "Program ID is 0");

	return cacheTextureUniforms() 
		  && cacheCameraUniforms()
		  && cacheModelUniforms() 
		  && cacheLightUniforms();
}

bool UniformCache::getUniformLocation(int& location, const char* name) const {
	location = glGetUniformLocation(program, name);
	if (location < 0) return error("UniformCache", "getUniformLocation", std::string("Could not find uniform: ") + name);
	return true;
}
bool UniformCache::cacheTextureUniforms() {
	glUniform1i(glGetUniformLocation(program, "textSampler2D_00"), 0);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_01"), 1);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_02"), 2);
	glUniform1i(glGetUniformLocation(program, "textSampler2D_03"), 3);

	bool ok = true;
	ok &= getUniformLocation(modelUL.useTextures, "bUseTextures");
	ok &= getUniformLocation(modelUL.texMixRatios, "texMixRatios");
	ok &= getUniformLocation(skyboxTextureLocation, "skyboxCubeTexture");
	if (skyboxTextureLocation != -1) glUniform1i(skyboxTextureLocation, SKYBOX_TU);

	return ok;
}
bool UniformCache::cacheCameraUniforms() {
	return getUniformLocation(eyeLocation, "eyePos");
}
bool UniformCache::cacheModelUniforms() {
	bool ok = true;
	ok &= getUniformLocation(modelUL.isSkybox, "bIsSkybox");
	ok &= getUniformLocation(modelUL.model, "mModel");
	ok &= getUniformLocation(modelUL.modelView, "mView");
	ok &= getUniformLocation(modelUL.modelProj, "mProj");
	ok &= getUniformLocation(modelUL.modelInverseTranspose, "mModel_InverseTranspose");
	ok &= getUniformLocation(modelUL.colourMode, "colourMode");
	ok &= getUniformLocation(modelUL.hasVertexColour, "hasVertexColour");
	ok &= getUniformLocation(modelUL.colourOverride, "colourOverride");
	ok &= getUniformLocation(modelUL.specular, "vertSpecular");
	ok &= getUniformLocation(modelUL.yMinMax, "yMin_yMax");
	ok &= getUniformLocation(modelUL.seed, "seed");
	ok &= getUniformLocation(modelUL.isLit, "bIsLit");
	return ok;
}
bool UniformCache::cacheLightUniforms() {
	bool ok = true;
	ok &= getUniformLocation(lightCountLocation, "lightCount");
	ok &= getUniformLocation(ambientLightLocation, "ambientLight");
	ok &= getUniformLocation(lightUL.position_UL, "theLights[0].position");
	ok &= getUniformLocation(lightUL.diffuse_UL, "theLights[0].diffuse");
	ok &= getUniformLocation(lightUL.attenuation_UL, "theLights[0].attenuation");
	ok &= getUniformLocation(lightUL.direction_UL, "theLights[0].direction");
	ok &= getUniformLocation(lightUL.param1_UL, "theLights[0].param1");
	ok &= getUniformLocation(lightUL.param2_UL, "theLights[0].param2");
	return ok;
}