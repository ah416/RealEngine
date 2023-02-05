#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Buffer.h"

#include <string>

class Cube
{
public:
	Cube(const float size);

	Ref<VertexArray> CreateBuffers() const;

	float* GetVertices() const;
	float* GetNormals() const;
	float* GetTextureCoordinates() const;
	uint32_t* GetIndices() const;

	float GetSizeOfVertices() const { return sizeof(m_Vertices); }
	float GetSizeOfNormals() const { return sizeof(m_Normals); }
	float GetSizeOfTextureCoords() const { return sizeof(m_Vertices); }
	float GetSizeOfIndices() const { return sizeof(m_Indices); }
private:
	float m_Vertices[4 * 6 * 3];
	float m_Normals[4 * 6 * 3];

	float m_TextureCoords[2 * 4 * 6];
	uint32_t m_Indices[12 * 3];
};