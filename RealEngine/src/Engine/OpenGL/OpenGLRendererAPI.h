#pragma once

#include "Engine/Renderer/RendererAPI.h"

#include "Engine/Renderer/VertexArray.h"

#include <glm/glm.hpp>

class OpenGLRendererAPI : public RendererAPI
{
public:
	void SetClearColor(const glm::vec4& color);

	void Clear();

	void ResizeViewport(int x, int y, int width, int height);

	void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);

	void DrawArrays(int first, int count);
};