#pragma once

#include "Engine/Core/Application.h"

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "../libs/glfw/include/GLFW/glfw3.h"
#include <../libs/glfw/include/GLFW/glfw3native.h>
#endif

#include <filesystem>

struct File
{
	File(std::filesystem::path name, bool directory) : Name(name), Directory(directory)
	{
	}

	std::filesystem::path Name;
	bool Directory;
};

class Filesystem
{
public:
	static bool IsDirectory(std::filesystem::path path);
	
	static void OpenInFileExplorer(std::filesystem::path path);

	static std::filesystem::path OpenFileDialog(const char* filter = "All\0*.*\0");

	static std::vector<File> GetFiles(std::filesystem::path filepath);

	static std::string GetExt(const std::string& filename);
};