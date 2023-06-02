#include "realpch.h"

#include "EngineFilesystem.h"

#ifdef _WIN32
#include <ShlObj.h>
#else
#include <unistd.h>
#endif

bool Filesystem::IsDirectory(std::filesystem::path path)
{
	return std::filesystem::is_directory(path);
}

void Filesystem::OpenInFileExplorer(std::filesystem::path path)
{
	auto absolutePath = std::filesystem::canonical(path);

	std::string cmd = "explorer.exe /select,\"" + absolutePath.string() + "\"";
	int res = system(cmd.c_str());
	if (res == 0)
		REAL_ERROR("system command {} failed!", cmd);
}

std::filesystem::path Filesystem::OpenFileDialog(const char* filter)
{
#ifdef _WIN32
	OPENFILENAMEA ofn;       // common dialog box structure
	CHAR szFile[260] = { 0 };       // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow()->GetNativeWindow());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		std::string fp = ofn.lpstrFile;
		std::replace(fp.begin(), fp.end(), '\\', '/');
		return std::filesystem::path(fp);
	}
#else
	char filename[1024];
	FILE* f = popen("zenity --file-selection", "r");
	fgets(filename, 1024, f);
	filename[strcspn(filename, "\n")] = 0;

	if (strcmp(filename, "") == 0)
		return std::filesystem::path();
	return std::filesystem::path(std::string(filename));
#endif // _WIN32

	return std::filesystem::path();
}

std::vector<File> Filesystem::GetFiles(std::filesystem::path filepath)
{
	std::vector<File> files;
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		if (entry.exists())
		{
			files.push_back(File(entry.path(), Filesystem::IsDirectory(entry.path())));
		}
	}
	return files;
}

std::string Filesystem::GetExt(const std::string& filename)
{
	if (filename.find('.') != std::string::npos)
		return strrchr(filename.c_str(), '.');
	else
		return "folder";
}
