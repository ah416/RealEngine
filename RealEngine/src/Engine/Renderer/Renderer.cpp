#include "realpch.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/PerspectiveCamera.h"
#include "Engine/Renderer/OrthographicCamera.h"

#include "Engine/Platform/OpenGL/OpenGLRendererAPI.h"

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

Renderer::RenderData* Renderer::s_RenderData = new Renderer::RenderData();

std::vector<Ref<Shader>> Renderer::m_Shaders;

void Renderer::LoadShaders()
{
	Ref<Shader> texture_shader;
	std::string vertexShader = Shader::ReadShader("../RealEngine/Resources/Shaders/TextureVertex.shader");
	std::string fragShader = Shader::ReadShader("../RealEngine/Resources/Shaders/TextureFrag.shader");
	texture_shader.reset(new Shader(vertexShader, fragShader));
	m_Shaders.push_back(texture_shader);
}

void Renderer::BeginScene(PerspectiveCamera& camera)
{
	PROFILE_FUNCTION();

	m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	m_SceneData->CameraPosition = camera.GetCameraPos();

	s_RenderData->DrawCalls = 0;
	s_RenderData->Indices = 0;
	s_RenderData->Vertices = 0;
}

void Renderer::BeginScene(OrthographicCamera& camera)
{
	PROFILE_FUNCTION();

	m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

	s_RenderData->DrawCalls = 0;
	s_RenderData->Indices = 0;
	s_RenderData->Vertices = 0;
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const Ref<Mesh>& mesh, const glm::mat4& transform)
{
	for (auto& shader : m_Shaders)
	{
		shader->Bind();

		mesh->MeshMaterial->DiffuseTex->Bind();
		shader->UploadUniformInt1("u_Material.diffuse", 0);
		mesh->MeshMaterial->NormalTex->Bind(1);
		shader->UploadUniformInt1("u_Material.normal", 1);

		shader->UploadUniformFloat3("u_Light.position", { 1.0f, 1.0f, 1.0f });
		shader->UploadUniformFloat3("u_Light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		shader->UploadUniformFloat3("u_Light.ambient", { 0.1f, 0.1f, 0.1f });
		
		shader->UploadUniformFloat3("u_ViewPos", m_SceneData->CameraPosition);

		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform);
	}

	s_RenderData->Vertices = mesh->VertexCount;
	s_RenderData->Indices = mesh->IndexCount;
	for (auto& submesh : mesh->m_Submeshes)
	{
		submesh.GetVertexArray()->Bind();

		submesh.GetVertexArray()->GetIndexBuffer()->Bind();
		RenderCommand::DrawIndexed(submesh.GetVertexArray());
		s_RenderData->DrawCalls += 1;

		submesh.GetVertexArray()->GetIndexBuffer()->Unbind();
		submesh.GetVertexArray()->Unbind();
	}

	for (auto& shader : m_Shaders)
		shader->Unbind();

	mesh->MeshMaterial->DiffuseTex->Unbind();
	mesh->MeshMaterial->NormalTex->Unbind();
	mesh->MeshMaterial->SpecularTex->Unbind();
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
{
	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	shader->UploadUniformMat4("u_Transform", transform);

	vertexArray->Bind();
	for (auto& i : vertexArray->GetVertexBuffers())
	{
		i->Bind();
	}
	vertexArray->GetIndexBuffer()->Bind();

	RenderCommand::DrawIndexed(vertexArray);
	s_RenderData->DrawCalls += 1;

	shader->Unbind();
	vertexArray->Unbind();
	for (auto& i : vertexArray->GetVertexBuffers())
	{
		i->Unbind();
	}
	vertexArray->GetIndexBuffer()->Unbind();
}

// Draw submesh, used for debugging only
void Renderer::Submit(const std::shared_ptr<Shader>& shader, const Submesh& mesh, const glm::mat4& transform)
{
	//mesh->GetMaterial()->DiffuseTex->Bind();
	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	shader->UploadUniformMat4("u_Transform", transform);

	std::shared_ptr<VertexArray> vertexArray = mesh.GetVertexArray();

	vertexArray->Bind();
	for (auto& i : vertexArray->GetVertexBuffers())
	{
		i->Bind();
	}
	vertexArray->GetIndexBuffer()->Bind();

	RenderCommand::DrawIndexed(mesh.GetVertexArray());
	s_RenderData->DrawCalls += 1;
	s_RenderData->Vertices += (uint64_t)mesh.VertexCount;

	shader->Unbind();
	vertexArray->Unbind();
	for (auto& i : vertexArray->GetVertexBuffers())
	{
		i->Unbind();
	}
	vertexArray->GetIndexBuffer()->Unbind();

}

// Used to draw a mesh with submeshes, will be replaced by function without shader param
void Renderer::Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform)
{
	mesh->MeshMaterial->DiffuseTex->Bind();
	mesh->MeshMaterial->NormalTex->Bind(1);
	mesh->MeshMaterial->SpecularTex->Bind(2);

	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	shader->UploadUniformMat4("u_Transform", transform);

	s_RenderData->Vertices = mesh->VertexCount;
	s_RenderData->Indices = mesh->IndexCount;
	for (auto& submesh : mesh->m_Submeshes)
	{
		submesh.GetVertexArray()->Bind();
		for (auto& i : submesh.GetVertexArray()->GetVertexBuffers())
		{
			i->Bind();
		}

		submesh.GetVertexArray()->GetIndexBuffer()->Bind();
		RenderCommand::DrawIndexed(submesh.GetVertexArray());
		s_RenderData->DrawCalls += 1;

		for (auto& i : submesh.GetVertexArray()->GetVertexBuffers())
		{
			i->Unbind();
		}
		submesh.GetVertexArray()->GetIndexBuffer()->Unbind();
		submesh.GetVertexArray()->Unbind();
	}
	shader->Unbind();

	mesh->MeshMaterial->DiffuseTex->Unbind();
	mesh->MeshMaterial->NormalTex->Unbind();
	mesh->MeshMaterial->SpecularTex->Unbind();
}

/*
void Renderer::Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform)
{
	//mesh->GetMaterial()->DiffuseTex->Bind();
	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	shader->UploadUniformMat4("u_Transform", transform);

	s_RenderData->Vertices = mesh->VertexCount;
	s_RenderData->Indices = mesh->IndexCount;

	mesh->GetVertexArray()->Bind();
	for (auto& i : mesh->GetVertexArray()->GetVertexBuffers())
	{
		i->Bind();
	}

	mesh->GetVertexArray()->GetIndexBuffer()->Bind();
	RenderCommand::DrawIndexed(mesh->GetVertexArray());
	s_RenderData->DrawCalls += 1;

	for (auto& i : mesh->GetVertexArray()->GetVertexBuffers())
	{
		i->Unbind();
	}
	mesh->GetVertexArray()->GetIndexBuffer()->Unbind();
	mesh->GetVertexArray()->Unbind();

	shader->Unbind();

	//mesh->GetMaterial()->DiffuseTex->Unbind();
}
*/

/*
void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh, bool multiple, const glm::mat4& transform)
{
	mesh->GetMaterial()->DiffuseTex->Bind();
	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	shader->UploadUniformMat4("u_Transform", transform);

	for (auto& vertexArray : mesh->GetVertexArrays())
	{
		vertexArray->Bind();
		for (auto& i : vertexArray->GetVertexBuffers())
		{
			i->Bind();
		}
		vertexArray->GetIndexBuffer()->Bind();

		RenderCommand::DrawIndexed(vertexArray);
		s_RenderData->DrawCalls += 1;

		vertexArray->Unbind();
		for (auto& i : vertexArray->GetVertexBuffers())
		{
			i->Unbind();
		}
		vertexArray->GetIndexBuffer()->Unbind();
	}
	shader->Unbind();
	mesh->GetMaterial()->DiffuseTex->Unbind();
}
*/

void Renderer::DrawTextured(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform)
{
}

void Renderer::DrawLine(const glm::vec3& start, const glm::vec3& end)
{
}

void Renderer::DrawPoint(const glm::vec3& location, float radius)
{
}

void Renderer::DrawArrays(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const int first, const int count)
{
	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	shader->UploadUniformMat4("u_Transform", glm::mat4(1.0f));

	vertexArray->Bind();
	vertexArray->GetVertexBuffers()[0]->Bind();

	RenderCommand::DrawArrays(first, count);
	s_RenderData->DrawCalls += 1;

	vertexArray->Unbind();
	vertexArray->GetVertexBuffers()[0]->Unbind();
	shader->Unbind();
}
