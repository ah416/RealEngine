#include "realpch.h"

#include "ImGuiLayer.h"

#include "Core/Application.h"

#include "imgui.h"
#include "Engine/OpenGL/ImGuiImpl/imgui_impl_opengl3.h"
//#include "Engine/Vulkan/ImGuiImpl/imgui_impl_vulkan.h"
#include "Engine/Platform/imgui_impl_glfw.h"

struct GLFWwindow;

ImGuiLayer::ImGuiLayer()
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();

	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Application::Get().GetWindow()->GetNativeWindow(), true);
	//ImGui_ImplVulkan_Init();
	ImGui_ImplOpenGL3_Init("#version 410");

	auto& style = ImGui::GetStyle();
	auto& colors = style.Colors;

	io.Fonts->AddFontFromFileTTF("../RealEngine/src/Engine/OpenGL/ImGuiImpl/Fonts/CascadiaCode.ttf", 16.0f);

	//========================================================
	/// Colors

	// Headers
	colors[ImGuiCol_Header] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::groupHeader);
	colors[ImGuiCol_HeaderHovered] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::groupHeader);
	colors[ImGuiCol_HeaderActive] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::groupHeader);

	// Buttons
	colors[ImGuiCol_Button] = ImColor(30, 30, 30, 200);
	colors[ImGuiCol_ButtonHovered] = ImColor(50, 50, 70, 255);
	colors[ImGuiCol_ButtonActive] = ImColor(70, 70, 70, 150);

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::propertyField);
	colors[ImGuiCol_FrameBgHovered] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::propertyField);
	colors[ImGuiCol_FrameBgActive] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::propertyField);

	// Tabs
	colors[ImGuiCol_Tab] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::titlebar);
	colors[ImGuiCol_TabHovered] = ImColor(255, 225, 135, 30);
	colors[ImGuiCol_TabActive] = ImColor(255, 225, 135, 60);
	colors[ImGuiCol_TabUnfocused] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::titlebar);
	colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TabHovered];

	// Title
	colors[ImGuiCol_TitleBg] = ImColor(40, 40, 40, 255);
	colors[ImGuiCol_TitleBgActive] = ImColor(20, 20, 20, 255);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Resize Grip
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

	// Check Mark
	colors[ImGuiCol_CheckMark] = ImColor(200, 200, 200, 255);

	// Slider
	colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);

	// Text
	colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::text);

	// Checkbox
	colors[ImGuiCol_CheckMark] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::text);

	// Separator
	colors[ImGuiCol_Separator] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::backgroundDark);
	colors[ImGuiCol_SeparatorActive] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::highlight);
	colors[ImGuiCol_SeparatorHovered] = ImColor(39, 185, 242, 150);

	// Window Background
	colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::titlebar);
	colors[ImGuiCol_ChildBg] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::background);
	colors[ImGuiCol_PopupBg] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::backgroundPopup);
	colors[ImGuiCol_Border] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::backgroundDark);

	// Tables
	colors[ImGuiCol_TableHeaderBg] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::groupHeader);
	colors[ImGuiCol_TableBorderLight] = ImGui::ColorConvertU32ToFloat4(Colours::Theme::backgroundDark);

	// Menubar
	colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	//========================================================
	/// Style
	style.FrameRounding = 2.5f;
	style.FrameBorderSize = 1.0f;
	style.IndentSpacing = 11.0f;
}

void ImGuiLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
	PROFILE_FUNCTION();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::End()
{
	PROFILE_FUNCTION();

	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2((float)app.GetWindow()->GetWidth(), (float)app.GetWindow()->GetHeight());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& e)
{
	ImGuiIO& io = ImGui::GetIO();

	e.Handled |= io.WantCaptureMouse;
	e.Handled |= io.WantCaptureKeyboard;
}
