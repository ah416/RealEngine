#pragma once

#ifndef _WIN32
extern Application* CreateApplication();

int main()
{
	Logger::Init();

	PROFILE_BEGIN_SESSION("Startup", "Startup-Results.json");
	Application* app = CreateApplication();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Runtime", "Runtime-Results.json");
	app->Run();
	PROFILE_END_SESSION();

	delete app;
	return 0;
}

#else

#include <Windows.h>
#include <iostream>

extern Application* CreateApplication();

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	Logger::Init();

	PROFILE_BEGIN_SESSION("Startup", "Startup-Results.json");
	Application* app = CreateApplication();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Runtime", "Runtime-Results.json");
	app->Run();
	PROFILE_END_SESSION();

	delete app;

	return 0;
}

#endif