#include "realpch.h"

#include "Engine/Renderer/Mesh/Mesh.h"

#include "Engine/Renderer/Mesh/Submesh.h"
#include "Engine/Tools/MeshLoader.h"

Mesh::Mesh(const std::string& filepath) : VertexCount(0), IndexCount(0), MeshMaterial(new ::Material("")), m_Filepath(filepath)
{
	REAL_ASSERT(MeshLoader::Load(filepath, this), "Failed loading file!");
	m_FullVertices.clear();
	m_Indices.clear();
}

void Mesh::Scale(float scale)
{
	PROFILE_FUNCTION();

	REAL_WARN("Meshes aren't loaded... (Mesh::Scale)");
	for (auto& mesh : m_Submeshes)
	{
		for (auto& vertex : mesh.Vertices)
		{
			vertex.Position *= scale;
		}

		REAL_ASSERT(mesh.Create(), "Mesh create in Scale() failed!");
	}
}
