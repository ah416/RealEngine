#pragma once

class GraphicsContext
{
public:
	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;
	virtual void VSync(bool enable) = 0;
	virtual void EnableDepth(bool enable) = 0;

	static GraphicsContext* Create(void* window);
};