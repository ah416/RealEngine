workspace "RealEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "RealEngine/libs/glfw"
include "RealEngine/libs/glad"
include "RealEngine/libs/imgui"

project "RealEngine"
	location "RealEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	systemversion "latest"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "realpch.h"
	pchsource "RealEngine/src/realpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/libs/glm/glm/**.hpp",
		"%{prj.name}/libs/glm/glm/**.inl",
		"%{prj.name}/libs/stb_image/**.cpp",
		"%{prj.name}/libs/stb_image/**.h",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/Engine",
		"%{prj.name}/libs/glfw/include",
		"%{prj.name}/libs/glad/include",
		"%{prj.name}/libs/imgui",
		"%{prj.name}/libs/glm",
		"%{prj.name}/libs/FastNoiseLite",
		"%{prj.name}/libs/stb_image",
		--"%{prj.name}/libs/assimp/include"
		"/home/adam/VulkanSDK/1.3.239.0/x86_64/include"
	}

	links 
	{
		"GLFW",
		"glad",
		"imgui",
		"opengl32.lib",
		"vulkan", -- <- download VulkanSDK and run the setup script for the lib to be in PATH
		--"/home/adam/VulkanSDK/1.3.239.0/lib/vulkan-1.lib",
		"assimp" -- <- this works only on linux with libassimp-dev installed
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
	}

	filter "system:linux"
		defines "REAL_LINUX"

	filter "system:windows"
		defines { "REAL_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }

		includedirs
		{
			"C:\\VulkanSDK\\Version\\Include",
			"%{prj.name}\\libs\\assimp\\include"
		}

		links
		{
			"C:\\VulkanSDK\\1.3.239.0\\Lib\\vulkan-1.lib",
			"%{prj.name}\\libs\\assimp\\bin\\Release\\assimp_mt.lib"
		}

	filter "configurations:Debug"
		defines "REAL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "REAL_RELEASE"
		optimize "On"

project "TestApp"
	location "TestApp"
	kind "ConsoleApp"
	cppdialect "C++20"
	language "C++"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"RealEngine/src",
		"RealEngine/libs/glm",
		"RealEngine/libs/imgui",
		"RealEngine/libs/FastNoiseLite",
		"RealEngine/libs/stb_image"
	}

	filter "system:linux"
		links
		{
			"RealEngine",
			"glad",
			"glfw",
			"imgui",
			"assimp"
		}

	filter "system:windows"
		linkoptions { "/ENTRY:WinMainCRTStartup" }

		postbuildcommands 
		{
			'{COPY} "$(SolutionDir)RealEngine/libs/assimp/bin/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"' -- <--- required by assimp
		}

		links
		{
			"RealEngine"
		}

	filter "configurations:Debug"
		defines "REAL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "REAL_RELEASE"
		optimize "On"
