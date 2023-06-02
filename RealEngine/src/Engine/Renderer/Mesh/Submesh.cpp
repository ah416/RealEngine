#include "realpch.h"

#include "Submesh.h"
#include "Engine/Tools/MeshLoader.h"
#include <assimp/Importer.hpp>

Submesh::Submesh(const Ref<VertexArray> va) {
	m_VertexArray = va;
}

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
	REAL_INFO("Mesh {}", this->MeshName);
	REAL_INFO(" - Vertex count: {}", Vertices.size());
	REAL_INFO(" - Index count: {}", Indices.size() * 3);

	m_VertexArray->AddVertexBuffer(vertexVB);
	m_VertexArray->SetIndexBuffer(indexIB);
	
	//Vertices.clear();
	//Indices.clear();
	return true;
}
