#pragma once

#include "Engine/OpenGL/OpenGLRendererAPI.h"

class RenderCommand
{
public:
	static inline void SetClearColor(const glm::vec4& color) { s_API->SetClearColor(color); }

	static inline void Clear() { s_API->Clear(); }

	static inline void ResizeViewport(int x, int y, int width, int height) { s_API->ResizeViewport(x, y, width, height); }

	static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { s_API->DrawIndexed(vertexArray); }

	static inline void DrawArrays(const int first, const int count) { s_API->DrawArrays(first, count); }
private:
	static OpenGLRendererAPI* s_API;
};