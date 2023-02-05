#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Mesh/Mesh.h"

#include <string>

class MeshLoader
{
	MeshLoader() = delete;
public:
	static bool Load(const std::string& filepath, Mesh* pMesh);
};