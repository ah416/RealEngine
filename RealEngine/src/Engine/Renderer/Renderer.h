#pragma once

#include "Engine/Core/Core.h"

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Mesh/Mesh.h"
#include "Engine/Renderer/Mesh/Submesh.h"

class PerspectiveCamera;
class OrthographicCamera;

class Renderer
{
	struct RenderData
	{
		RenderData() { Vertices = 0, Indices = 0, DrawCalls = 0;  }
		RenderData(uint64_t v, uint64_t i, uint32_t dc) { Vertices = v, Indices = i, DrawCalls = dc; }

		uint64_t Vertices;
		uint64_t Indices;
		uint32_t DrawCalls;
	};

public:
	// Must be called before any rendering occurs. Loads shaders into memory.
	static void LoadShaders();

	// Begins the scene, and transfers the vp matrix from the camera to scene data
	static void BeginScene(PerspectiveCamera& camera);

	// Begins the scene, and transfers the vp matrix from the camera to scene data
	static void BeginScene(OrthographicCamera& camera);

	// Ends the current frame, currently does nothing
	static void EndScene();

	// Currently used method for submitting a mesh to the gpu
	static void Submit(const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));

	// Submits a draw call of a shader, VA, and transform for that VA
	static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	// Submits a draw call of a shader, mesh (VA), and transform for that mesh
	static void Submit(const std::shared_ptr<Shader>& shader, const Submesh& mesh, const glm::mat4& transform = glm::mat4(1.0f));

	// Submits a draw call of a shader, mesh, and transform for that mesh
	static void Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));

	// Draw a textured mesh
	static void DrawTextured(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));

	// NOT IMPLEMENTED
	static void DrawLine(const glm::vec3& start, const glm::vec3& end);

	// NOT IMPLEMENTED
	static void DrawPoint(const glm::vec3& location, float radius);

	// Should only be used for testing, doesn't draw with indices
	static void DrawArrays(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const int first, const int count);

	static RenderData& GetRenderData() { return *s_RenderData; }

private:
	struct SceneData
	{
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewProjectionMatrix;

		glm::vec3 CameraPosition;
	};

	// Holds submitted camera mvp for use in submissions
	static SceneData* m_SceneData;

	static RenderData* s_RenderData;

	static std::vector<Ref<Shader>> m_Shaders;
};