#pragma once

#include "Engine/Renderer/VertexArray.h"

#include <glm/glm.hpp>

class RendererAPI
{
public:
	virtual void SetClearColor(const glm::vec4& color) = 0;

	virtual void Clear() = 0;

	virtual void ResizeViewport(int x, int y, int width, int height) = 0;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& va) = 0;

	virtual void DrawArrays(int first, int count) = 0;

	static RendererAPI* Create();
};