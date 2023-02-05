#include "realpch.h"

#include "Submesh.h"
#include "Engine/Tools/MeshLoader.h"
#include <assimp/Importer.hpp>

bool Submesh::Create()
{
	PROFILE_FUNCTION();

	m_VertexArray.reset(VertexArray::Create());

	std::shared_ptr<VertexBuffer> vertexVB;
	std::shared_ptr<IndexBuffer> indexIB;

	vertexVB.reset(VertexBuffer::Create(Vertices.data(), (uint32_t)Vertices.size() * sizeof(Mesh::Vertex)));
	vertexVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float3, "a_Normal" },
		{ ShaderDataType::Float3, "a_Tangent" },
		{ ShaderDataType::Float3, "a_Binormal" },
		{ ShaderDataType::Float2, "a_TexCoord" }
		});

	indexIB.reset(IndexBuffer::Create(Indices.data(), (uint32_t)Indices.size() * sizeof(Mesh::Index)));
	REAL_INFO("Mesh {0}", this->MeshName);
	REAL_INFO(" - Vertex count: {0}", Vertices.size());
	REAL_INFO(" - Index count: {0}", Indices.size() * 3);

	m_VertexArray->AddVertexBuffer(vertexVB);
	m_VertexArray->SetIndexBuffer(indexIB);

	return true;
}
