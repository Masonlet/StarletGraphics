#pragma once

#include <string>

class ResourceManager {
public:
	ResourceManager() = default;
	virtual ~ResourceManager() = default;

	void setBasePath(const std::string& path) { basePath = path; }
	const std::string& getBasePath() const { return basePath; }

	virtual bool exists(const std::string& name) const = 0;

protected:
	std::string basePath;
};