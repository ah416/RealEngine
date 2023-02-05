#include "realpch.h"

#include "MeshLoader.h"

#include "Engine/Renderer/Mesh/Submesh.h"
#include "Engine/Renderer/VertexArray.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

struct LogStream : public Assimp::LogStream
{
	static void Initialize()
	{
		if (Assimp::DefaultLogger::isNullLogger())
		{
			Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
			Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
		}
	}

	virtual void write(const char* message) override
	{
		PROFILE_FUNCTION();

		REAL_WARN("Assimp: {0}", message);
	}
};

bool MeshLoader::Load(const std::string& filepath, Mesh* pMesh)
{
	PROFILE_FUNCTION();

	const uint32_t MeshImportFlags =
		aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
		aiProcess_Triangulate |             // Make sure we're triangles
		aiProcess_SortByPType |             // Split meshes by primitive type
		aiProcess_GenNormals |              // Make sure we have legit normals
		aiProcess_GenUVCoords |             // Convert UVs if required 
		aiProcess_OptimizeMeshes |          // Batch draws where possible
		aiProcess_JoinIdenticalVertices |   // Optimize indices
		aiProcess_GlobalScale |             // e.g. convert cm to m for fbx import (and other formats where cm is native)
		aiProcess_ValidateDataStructure;    // Validation

	LogStream::Initialize();

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filepath, MeshImportFlags);
	if (!scene->HasMeshes())
		REAL_WARN("Scene {0} contained no meshes to load!", scene->GetShortFilename(filepath.c_str()));

	pMesh->m_Name = scene->mName.C_Str();

	unsigned vertexCount = 0, indexCount = 0;
	pMesh->m_Submeshes.reserve(scene->mNumMeshes);

	for (unsigned m = 0; m < scene->mNumMeshes; m++)
	{
		aiMesh* mesh = scene->mMeshes[m];

		Submesh& submesh = pMesh->m_Submeshes.emplace_back();
		submesh.BaseVertex = vertexCount;
		submesh.BaseIndex = indexCount;
		//submesh.MaterialIndex = mesh->mMaterialIndex;
		submesh.VertexCount = mesh->mNumVertices;
		submesh.IndexCount = mesh->mNumFaces * 3;
		submesh.MeshName = mesh->mName.C_Str();

		pMesh->VertexCount += mesh->mNumVertices;
		pMesh->IndexCount += submesh.IndexCount;

		REAL_ASSERT(mesh->HasPositions(), "Meshes require positions.");
		REAL_ASSERT(mesh->HasNormals(), "Meshes require normals.");

		// Vertices
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Mesh::Vertex vertex;
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}

			if (mesh->HasTextureCoords(0))
				vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

			submesh.Vertices.push_back(vertex);

			pMesh->m_FullVertices.push_back(vertex);
		}

		// Indices
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			REAL_ASSERT((mesh->mFaces[i].mNumIndices == 3), "Must have 3 indices.");
			Mesh::Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
			submesh.Indices.push_back(index);

			pMesh->m_Indices.push_back(index);
			//m_TriangleCache[m].emplace_back(m_StaticVertices[index.V1 + submesh.BaseVertex], m_StaticVertices[index.V2 + submesh.BaseVertex], m_StaticVertices[index.V3 + submesh.BaseVertex]);
		}
		submesh.Create();
	}

	Ref<VertexBuffer> vertexBuf;
	vertexBuf.reset(VertexBuffer::Create(pMesh->m_FullVertices.data(), (uint32_t)pMesh->m_FullVertices.size() * sizeof(Mesh::Vertex)));
	vertexBuf->SetLayout({
		{ ShaderDataType::Float3, "Vertices" },
		{ ShaderDataType::Float3, "Normals" },
		{ ShaderDataType::Float3, "Tangents" },
		{ ShaderDataType::Float3, "Binormals" },
		{ ShaderDataType::Float2, "UVs" }
		});
	pMesh->m_FullMesh.reset(VertexArray::Create());
	pMesh->m_FullMesh->AddVertexBuffer(vertexBuf);

	Ref<IndexBuffer> indexBuf;
	indexBuf.reset(IndexBuffer::Create(pMesh->m_Indices.data(), (uint32_t)pMesh->m_Indices.size() * sizeof(Mesh::Index)));
	pMesh->m_FullMesh->SetIndexBuffer(indexBuf);
	return true;
}
