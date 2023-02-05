#include "realpch.h"

#include "Material.h"

Material::Material()
	: Name("Unnamed"), Diffuse_Map("../RealEngine/textures/brick.jpg"), Specular_Map("../RealEngine/textures/brick.jpg"), Normal_Map("../RealEngine/textures/brick.jpg")
{
	DiffuseTex.reset(Texture2D::Create(Diffuse_Map));
	NormalTex.reset(Texture2D::Create(Normal_Map));
	SpecularTex.reset(Texture2D::Create(Specular_Map));
}

Material::Material(const std::string& name)
	: Name(name), Diffuse_Map("../RealEngine/textures/brick.jpg"), Specular_Map("../RealEngine/textures/brick.jpg"), Normal_Map("../RealEngine/textures/brick.jpg")
{
	DiffuseTex.reset(Texture2D::Create(Diffuse_Map));
	NormalTex.reset(Texture2D::Create(Normal_Map));
	SpecularTex.reset(Texture2D::Create(Specular_Map));
}

Material::Material(const std::string& name, const std::string& diffuse, const std::string& specular, const std::string& normal)
	: Name(name), Diffuse_Map(diffuse), Specular_Map(specular), Normal_Map(normal)
{
	DiffuseTex.reset(Texture2D::Create(Diffuse_Map));
	NormalTex.reset(Texture2D::Create(Normal_Map));
	SpecularTex.reset(Texture2D::Create(Specular_Map));
}
