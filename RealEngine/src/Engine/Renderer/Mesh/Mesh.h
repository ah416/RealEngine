#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/VertexArray.h"

#include <glm/glm.hpp>

class Submesh;
class Material;

class Mesh
{
	friend class MeshLoader;
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Binormal;
		glm::vec2 Texcoord;
	};
	struct Index
	{
		uint32_t V1, V2, V3;
	};
public:
	Mesh(const std::string& filepath);

	void Scale(float scale);

	Ref<VertexArray> GetVertexArray() const { return m_FullMesh; }

	uint64_t VertexCount;
	uint64_t IndexCount;

	std::vector<Submesh> m_Submeshes;

	Ref<Material> MeshMaterial;
private:
	std::vector<Vertex> m_FullVertices;
	std::vector<Index> m_Indices;
	Ref<VertexArray> m_FullMesh;
	std::string m_Name;
};