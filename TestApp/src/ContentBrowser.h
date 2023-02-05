#pragma once

#include <RealEngine.h>
#include "Engine/Tools/EngineFilesystem.h"

#include <filesystem>

class EditorPanel
{
public:
	enum class Type
	{
		None = 0, ContentBrowser
	};

public:
	virtual void OnImGuiRender() = 0;
};

class ContentBrowser : public EditorPanel
{
public:
	ContentBrowser()
	{
		m_FileImg.reset(Texture2D::Create("../RealEngine/textures/brick.jpg"));

		m_FileImgs[".json"].reset(Texture2D::Create("../RealEngine/Resources/json_file.png"));
		m_FileImgs[".cpp"].reset(Texture2D::Create("../RealEngine/Resources/cpp.png"));
		m_FileImgs[".h"].reset(Texture2D::Create("../RealEngine/Resources/header_file.png"));
		m_FileImgs["folder"].reset(Texture2D::Create("../RealEngine/Resources/folder_light.png"));

		m_Files = Filesystem::GetFiles(std::filesystem::current_path());
	}

	virtual void OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (ImGui::Button("OpenFileExplorer"))
			Filesystem::OpenInFileExplorer(std::filesystem::current_path());
		if (ImGui::BeginTable("table1", 3))
		{
			ImGui::TableNextColumn();
			int index = 0;
			for (auto& file : m_Files)
			{
				if (index >= 3)
				{
					ImGui::TableNextColumn();
					index = 0;
				}
				std::string ext = Filesystem::GetExt(file.Name.string());
				if (m_FileImgs.find(ext) != m_FileImgs.end())
					ImGui::Image((void*)m_FileImgs[ext]->GetRendererID(), {(float)m_FileImgs[ext]->GetWidth(), (float)m_FileImgs[ext]->GetHeight()}, {0, 1}, {1, 0});
				else
					ImGui::Image((void*)m_FileImg->GetRendererID(), { (float)m_FileImg->GetWidth() / 15, (float)m_FileImg->GetHeight() / 15 }, { 0, 1 }, { 1, 0 });
				ImGui::Text(std::filesystem::relative(file.Name).string().c_str());
				index++;
			}
			ImGui::EndTable();
		}

		ImGui::End();
	}

private:
	Ref<Texture2D> m_FileImg;
	std::vector<File> m_Files;
	std::unordered_map<std::string, Ref<Texture2D>> m_FileImgs;
};