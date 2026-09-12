#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "ImGuizmo.h"

#define IMGUI_IMPL_API
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Ripple/Core/Application.h"

// 临时↓
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Ripple {

    namespace {
        void ApplyDarkTheme()
        {
            ImGuiStyle& style = ImGui::GetStyle();
            ImGui::StyleColorsDark();

            // Apply logical sizes before OnAttach scales them for the monitor.
            style.WindowPadding = ImVec2(8.0f, 8.0f);
            style.FramePadding = ImVec2(6.0f, 4.0f);
            style.ItemSpacing = ImVec2(8.0f, 5.0f);
            style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
            style.CellPadding = ImVec2(6.0f, 4.0f);
            style.WindowRounding = 0.0f;
            style.ChildRounding = 0.0f;
            style.FrameRounding = 2.0f;
            style.PopupRounding = 3.0f;
            style.ScrollbarRounding = 3.0f;
            style.GrabRounding = 2.0f;
            style.TabRounding = 2.0f;
            style.WindowBorderSize = 1.0f;
            style.ChildBorderSize = 1.0f;
            style.PopupBorderSize = 1.0f;
            style.FrameBorderSize = 0.0f;
            style.TabBorderSize = 0.0f;
            style.TabBarBorderSize = 1.0f;
            style.ScrollbarSize = 12.0f;
            style.GrabMinSize = 10.0f;
            style.DockingSeparatorSize = 3.0f;

            const auto gray = [](int value, float alpha = 1.0f) {
                const float channel = value / 255.0f;
                return ImVec4(channel, channel, channel, alpha);
            };
            const ImVec4 accent(0.18f, 0.48f, 0.78f, 1.0f);
            const ImVec4 accentHovered(0.28f, 0.60f, 0.92f, 1.0f);
            const ImVec4 selection(0.16f, 0.28f, 0.40f, 1.0f);
            ImVec4* colors = style.Colors;

            // Neutral surfaces: recessed inputs, charcoal panels, quiet chrome.
            colors[ImGuiCol_Text] = gray(220);
            colors[ImGuiCol_TextDisabled] = gray(128);
            colors[ImGuiCol_WindowBg] = gray(30);
            colors[ImGuiCol_ChildBg] = gray(0, 0.0f);
            colors[ImGuiCol_PopupBg] = gray(30);
            colors[ImGuiCol_Border] = gray(50);
            colors[ImGuiCol_BorderShadow] = gray(0, 0.0f);
            colors[ImGuiCol_FrameBg] = gray(20);
            colors[ImGuiCol_FrameBgHovered] = gray(40);
            colors[ImGuiCol_FrameBgActive] = gray(48);
            colors[ImGuiCol_TitleBg] = gray(22);
            colors[ImGuiCol_TitleBgActive] = gray(26);
            colors[ImGuiCol_TitleBgCollapsed] = gray(22);
            colors[ImGuiCol_MenuBarBg] = gray(24);
            colors[ImGuiCol_ScrollbarBg] = gray(24);
            colors[ImGuiCol_ScrollbarGrab] = gray(60);
            colors[ImGuiCol_ScrollbarGrabHovered] = gray(80);
            colors[ImGuiCol_ScrollbarGrabActive] = gray(100);
            colors[ImGuiCol_CheckMark] = accentHovered;
            colors[ImGuiCol_SliderGrab] = gray(110);
            colors[ImGuiCol_SliderGrabActive] = accentHovered;
            colors[ImGuiCol_Button] = gray(40);
            colors[ImGuiCol_ButtonHovered] = gray(55);
            colors[ImGuiCol_ButtonActive] = gray(65);
            colors[ImGuiCol_Header] = selection;
            colors[ImGuiCol_HeaderHovered] = gray(55);
            colors[ImGuiCol_HeaderActive] = selection;
            colors[ImGuiCol_Separator] = gray(50);
            colors[ImGuiCol_SeparatorHovered] = accent;
            colors[ImGuiCol_SeparatorActive] = accentHovered;
            colors[ImGuiCol_ResizeGrip] = gray(110, 0.2f);
            colors[ImGuiCol_ResizeGripHovered] = accent;
            colors[ImGuiCol_ResizeGripActive] = accentHovered;
            colors[ImGuiCol_InputTextCursor] = gray(220);

            // Explicitly theme docking tabs so no default blue panels remain.
            colors[ImGuiCol_Tab] = gray(22);
            colors[ImGuiCol_TabHovered] = gray(40);
            colors[ImGuiCol_TabSelected] = gray(30);
            colors[ImGuiCol_TabSelectedOverline] = accent;
            colors[ImGuiCol_TabDimmed] = gray(22);
            colors[ImGuiCol_TabDimmedSelected] = gray(30);
            colors[ImGuiCol_TabDimmedSelectedOverline] = gray(70);
            colors[ImGuiCol_DockingPreview] = ImVec4(accent.x, accent.y, accent.z, 0.35f);
            colors[ImGuiCol_DockingEmptyBg] = gray(18);
            colors[ImGuiCol_PlotLines] = gray(170);
            colors[ImGuiCol_PlotLinesHovered] = accentHovered;
            colors[ImGuiCol_PlotHistogram] = accent;
            colors[ImGuiCol_PlotHistogramHovered] = accentHovered;
            colors[ImGuiCol_TableHeaderBg] = gray(36);
            colors[ImGuiCol_TableBorderStrong] = gray(50);
            colors[ImGuiCol_TableBorderLight] = gray(40);
            colors[ImGuiCol_TableRowBg] = gray(0, 0.0f);
            colors[ImGuiCol_TableRowBgAlt] = gray(255, 0.025f);
            colors[ImGuiCol_TextLink] = accentHovered;
            colors[ImGuiCol_TextSelectedBg] = selection;
            colors[ImGuiCol_TreeLines] = gray(60);
            colors[ImGuiCol_DragDropTarget] = accentHovered;
            colors[ImGuiCol_DragDropTargetBg] = ImVec4(accent.x, accent.y, accent.z, 0.15f);
            colors[ImGuiCol_UnsavedMarker] = gray(220);
            colors[ImGuiCol_NavCursor] = accentHovered;
            colors[ImGuiCol_NavWindowingHighlight] = gray(220, 0.7f);
            colors[ImGuiCol_NavWindowingDimBg] = gray(0, 0.55f);
            colors[ImGuiCol_ModalWindowDimBg] = gray(0, 0.6f);
        }
    }

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

    ImGuiLayer::ImGuiLayer(const std::string& name)
    {
    }
	
	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
        IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        ImFont* pFont = io.Fonts->AddFontFromFileTTF("assets/fonts/LXGWWenKai-Medium.ttf", 18.0f);
        io.FontDefault = io.Fonts->Fonts.back();

        ApplyDarkTheme();

        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
        // Setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410"); // 为ImGui内部定义的着色器代码添加一个版本号
	}

	void ImGuiLayer::OnDetach()
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End()
    {
		ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnImGuiRender()
    {
        //static bool show = true;
        //ImGui::ShowDemoWindow(&show);
    }

}
