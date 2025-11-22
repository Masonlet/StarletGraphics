#include "starlet-graphics/uniform/uniform_cache.hpp"
#include <glad/glad.h>

namespace Starlet::Graphics {
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
}