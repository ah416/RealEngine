#pragma once

#include "Engine/Renderer/Texture.h"

#include <string>

class Material
{
public:
	Material();
	Material(const std::string& name);
	Material(const std::string& name, const std::string& diffuse, const std::string& specular, const std::string& normal);
	~Material() {}

	Ref<Texture2D> DiffuseTex;
	Ref<Texture2D> NormalTex;
	Ref<Texture2D> SpecularTex;

	std::string Name;
	std::string Diffuse_Map;
	std::string Normal_Map;
	std::string Specular_Map;
};