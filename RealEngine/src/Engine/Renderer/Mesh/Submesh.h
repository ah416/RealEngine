#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh/Mesh.h"

#include <vector>

class Submesh
{
public:
	//Submesh(const std::string& filepath);
	//Submesh(const Ref<VertexArray> vertexArray);
	Submesh() { m_VertexArray.reset(VertexArray::Create()); }

	// Creates the Submesh from the Vertices and Indices specified
	bool Create();

	void AssignMaterial(Material* mat) { m_Material.reset(mat); }
	std::shared_ptr<Material> GetMaterial() const { return m_Material; }

	std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }

	std::vector<Mesh::Vertex> Vertices;
	std::vector<Mesh::Index> Indices;
	uint32_t BaseVertex = 0, BaseIndex = 0, VertexCount = 0, IndexCount = 0;
	std::string MeshName = "";
private:
	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<Texture2D> m_Texture;
	std::shared_ptr<Material> m_Material;

	std::string m_Filepath;
};
