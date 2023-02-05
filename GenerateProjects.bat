ROBOCOPY RealEngine\libs\premake_glfw RealEngine\libs\glfw premake5.lua
ROBOCOPY RealEngine\libs\premake_imgui RealEngine\libs\imgui premake5.lua
call premake5\premake5.exe vs2022
PAUSE