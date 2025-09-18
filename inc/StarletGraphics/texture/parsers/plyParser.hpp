#pragma once

struct Mesh;
#include <string>

bool parsePlyMesh(const std::string& path, Mesh& drawInfo);
