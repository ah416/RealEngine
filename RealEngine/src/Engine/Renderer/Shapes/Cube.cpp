#include "realpch.h"

#include "Cube.h"

/*

Spare texture coords for a cube
0.0, 0.0,
1.0, 0.0,
1.0, 1.0,
0.0, 1.0,

0.0, 0.0,
1.0, 0.0,
1.0, 1.0,
0.0, 1.0,

0.0, 0.0,
1.0, 0.0,
1.0, 1.0,
0.0, 1.0,

0.0, 0.0,
1.0, 0.0,
1.0, 1.0,
0.0, 1.0,

0.0, 0.0,
1.0, 0.0,
1.0, 1.0,
0.0, 1.0,

0.0, 0.0,
1.0, 0.0,
1.0, 1.0,
0.0, 1.0

*/

Cube::Cube(const float size)
{
	float vertices[] = {
		// front
	   -size, -size,  size,
		size, -size,  size,
		size,  size,  size,
	   -size,  size,  size,
		// top
	   -size,  size,  size,
		size,  size,  size,
		size,  size, -size,
	   -size,  size, -size,
		// back
		size, -size, -size,
	   -size, -size, -size,
	   -size,  size, -size,
		size,  size, -size,
		// bottom
	   -size, -size, -size,
		size, -size, -size,
		size, -size,  size,
	   -size, -size,  size,
		// left
	   -size, -size, -size,
	   -size, -size,  size,
	   -size,  size,  size,
	   -size,  size, -size,
		// right
		size, -size,  size,
		size, -size, -size,
		size,  size, -size,
		size,  size,  size
	};
	memcpy(&m_Vertices, &vertices, sizeof(vertices));
	
	float normals[] = {
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,

		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,

		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,

		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,

		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
	};
	memcpy(&m_Normals, &normals, sizeof(normals));

	// Holds coords for one face of cube
	float cube_texcoords[2 * 4 * 6] = {
		// front
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};
	// Remaps the coords for one face to the other 5
	for (int i = 1; i < 6; i++)
		memcpy(&cube_texcoords[i * 4 * 2], &cube_texcoords[0], 2 * 4 * sizeof(float));

	memcpy(&m_TextureCoords, &cube_texcoords, sizeof(cube_texcoords));

	uint32_t indices[] = {
		// front
		0,  1,  2,
		2,  3,  0,
		// top
		4,  5,  6,
		6,  7,  4,
		// back
		8,  9, 10,
		10, 11,  8,
		// bottom
		12, 13, 14,
		14, 15, 12,
		// left
		16, 17, 18,
		18, 19, 16,
		// right
		20, 21, 22,
		22, 23, 20,
	};

	memcpy(&m_Indices, &indices, sizeof(indices));
}

Ref<VertexArray> Cube::CreateBuffers() const
{
	Ref<VertexArray> va;
	va.reset(VertexArray::Create());

	std::shared_ptr<VertexBuffer> cubeVB;
	cubeVB.reset(VertexBuffer::Create(GetVertices(), (uint32_t)GetSizeOfVertices()));
	cubeVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" }
		});
	va->AddVertexBuffer(cubeVB);

	std::shared_ptr<VertexBuffer> texVB;
	texVB.reset(VertexBuffer::Create(GetTextureCoordinates(), (uint32_t)GetSizeOfTextureCoords()));
	texVB->SetLayout({
		{ ShaderDataType::Float2, "texCoords" }
		});
	va->AddVertexBuffer(texVB);

	std::shared_ptr<VertexBuffer> normalVB;
	normalVB.reset(VertexBuffer::Create(GetNormals(), (uint32_t)GetSizeOfNormals()));
	normalVB->SetLayout({
		{ ShaderDataType::Float3, "normals" }
		});
	va->AddVertexBuffer(normalVB);

	std::shared_ptr<IndexBuffer> cubeIB;
	cubeIB.reset(IndexBuffer::Create(GetIndices(), (uint32_t)GetSizeOfIndices() / sizeof(uint32_t)));

	va->SetIndexBuffer(cubeIB);

	return va;
}

float* Cube::GetVertices() const
{
	return (float*)m_Vertices;
}

float* Cube::GetNormals() const
{
	return (float*)m_Normals;
}

float* Cube::GetTextureCoordinates() const
{
	return (float*)m_TextureCoords;
}

uint32_t* Cube::GetIndices() const
{
	return (uint32_t*)m_Indices;
}
