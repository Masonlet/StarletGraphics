#pragma once

class UniformCache;
class Scene;

class LightRenderer {
public:
	LightRenderer(const UniformCache& uc) : uniforms(uc) {}

	void updateLightUniforms(const unsigned int program, const Scene& scene) const;
	void updateLightCount(const int count) const;
private:
	const UniformCache& uniforms;
};