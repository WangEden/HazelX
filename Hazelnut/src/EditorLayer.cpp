#include "EditorLayer.h"

#include "Hazel/ImGui/ImGuizmo.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Script/ScriptEngine.h"

#ifndef STB_TEXT_HAS_SELECTION
#define STB_TEXT_HAS_SELECTION(s) ((s)->select_start != (s)->select_end)
#endif

#include <imgui/imgui_internal.h>

namespace Hazel {

	static void ImGuiShowHelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	EditorLayer::EditorLayer()
		: m_SceneType(SceneType::Model), m_EditorCamera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f))
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		// ImGui Colors — 自定义深色主题
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
		colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.22f, 0.78f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.22f, 0.22f, 0.59f);
		colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
		colors[ImGuiCol_Separator] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.15f, 0.72f, 0.95f, 0.59f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.15f, 0.72f, 0.95f, 1.0f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.43f, 0.35f, 1.0f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.6f, 0.15f, 1.0f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
		colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.88f, 0.53f, 0.12f);
		colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.88f, 0.53f, 0.24f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.0f, 0.88f, 0.53f, 0.12f);

		// Tables
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

		// Popups
		colors[ImGuiCol_PopupBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

		// Style parameters
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 0.0f;
		style.FrameRounding = 2.5f;
		style.FrameBorderSize = 1.0f;
		style.TabRounding = 2.0f;
		style.ScrollbarRounding = 2.0f;
		style.GrabRounding = 2.0f;
		style.ChildRounding = 0.0f;
		style.PopupRounding = 2.0f;
		style.IndentSpacing = 11.0f;
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);

		using namespace glm;

		// Editor
		m_CheckerboardTex = Texture2D::Create("assets/editor/Checkerboard.tga");
		m_PlayButtonTex = Texture2D::Create("assets/editor/PlayButton.png");

		m_EditorScene = Ref<Scene>::Create();
		ScriptEngine::SetSceneContext(m_EditorScene);
		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(m_EditorScene);
		m_SceneHierarchyPanel->SetSelectionChangedCallback(std::bind(&EditorLayer::SelectEntity, this, std::placeholders::_1));
		m_SceneHierarchyPanel->SetEntityDeletedCallback(std::bind(&EditorLayer::OnEntityDeleted, this, std::placeholders::_1));
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnScenePlay()
	{
		m_SelectionContext.clear();

		m_SceneState = SceneState::Play;

		m_RuntimeScene = Ref<Scene>::Create();
		m_EditorScene->CopyTo(m_RuntimeScene);

		m_RuntimeScene->OnRuntimeStart();
		m_SceneHierarchyPanel->SetContext(m_RuntimeScene);
	}

	void EditorLayer::OnSceneStop()
	{
		m_RuntimeScene->OnRuntimeStop();
		m_SceneState = SceneState::Edit;

		m_RuntimeScene = nullptr;

		m_SelectionContext.clear();
		ScriptEngine::SetSceneContext(m_EditorScene);
		m_SceneHierarchyPanel->SetContext(m_EditorScene);
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				if (m_ViewportPanelFocused)
					m_EditorCamera.OnUpdate(ts);

				m_EditorScene->OnRenderEditor(ts, m_EditorCamera);

				if (m_DrawOnTopBoundingBoxes)
				{
					Renderer::BeginRenderPass(SceneRenderer::GetFinalRenderPass(), false);
					auto viewProj = m_EditorCamera.GetViewProjection();
					Renderer2D::BeginScene(viewProj, false);
					Renderer2D::EndScene();
					Renderer::EndRenderPass();
				}

				if (m_SelectionContext.size() && false)
				{
					auto& selection = m_SelectionContext[0];

					if (selection.Mesh && selection.Entity.HasComponent<MeshComponent>())
					{
						Renderer::BeginRenderPass(SceneRenderer::GetFinalRenderPass(), false);
						auto viewProj = m_EditorCamera.GetViewProjection();
						Renderer2D::BeginScene(viewProj, false);
						glm::vec4 color = (m_SelectionMode == SelectionMode::Entity) ? glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f } : glm::vec4{ 0.2f, 0.9f, 0.2f, 1.0f };
						Renderer::DrawAABB(selection.Mesh->BoundingBox, selection.Entity.GetComponent<TransformComponent>().Transform * selection.Mesh->Transform, color);
						Renderer2D::EndScene();
						Renderer::EndRenderPass();
					}
				}
				break;
			}
			case SceneState::Play:
			{
				if (m_ViewportPanelFocused)
					m_EditorCamera.OnUpdate(ts);

				m_RuntimeScene->OnUpdate(ts);
				m_RuntimeScene->OnRenderRuntime(ts);
				break;
			}
			case SceneState::Pause:
			{
				if (m_ViewportPanelFocused)
					m_EditorCamera.OnUpdate(ts);

				m_RuntimeScene->OnRenderRuntime(ts);
				break;
			}
		}
	}

	bool EditorLayer::Property(const std::string& name, bool& value)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		bool result = ImGui::Checkbox(id.c_str(), &value);

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return result;
	}

	void EditorLayer::Property(const std::string& name, float& value, float min, float max, EditorLayer::PropertyFlag flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		ImGui::SliderFloat(id.c_str(), &value, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void EditorLayer::Property(const std::string& name, glm::vec2& value, EditorLayer::PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void EditorLayer::Property(const std::string& name, glm::vec2& value, float min, float max, EditorLayer::PropertyFlag flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		ImGui::SliderFloat2(id.c_str(), glm::value_ptr(value), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void EditorLayer::Property(const std::string& name, glm::vec3& value, EditorLayer::PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void EditorLayer::Property(const std::string& name, glm::vec3& value, float min, float max, EditorLayer::PropertyFlag flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if ((int)flags & (int)PropertyFlag::ColorProperty)
			ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs);
		else
			ImGui::SliderFloat3(id.c_str(), glm::value_ptr(value), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void EditorLayer::Property(const std::string& name, glm::vec4& value, EditorLayer::PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void EditorLayer::Property(const std::string& name, glm::vec4& value, float min, float max, EditorLayer::PropertyFlag flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if ((int)flags & (int)PropertyFlag::ColorProperty)
			ImGui::ColorEdit4(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs);
		else
			ImGui::SliderFloat4(id.c_str(), glm::value_ptr(value), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void EditorLayer::ShowBoundingBoxes(bool show, bool onTop)
	{
		SceneRenderer::GetOptions().ShowBoundingBoxes = show && !onTop;
		m_DrawOnTopBoundingBoxes = show && onTop;
	}

	void EditorLayer::SelectEntity(Entity entity)
	{
		SelectedSubmesh selection;
		if (entity.HasComponent<MeshComponent>())
		{
			selection.Mesh = &entity.GetComponent<MeshComponent>().Mesh->GetSubmeshes()[0];
		}
		selection.Entity = entity;
		m_SelectionContext.clear();
		m_SelectionContext.push_back(selection);

		m_EditorScene->SetSelectedEntity(entity);
	}

	void EditorLayer::OnImGuiRender()
	{
		// === DockSpace Window ===
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		window_flags |= ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_MenuBar
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		// === Custom Titlebar ===
		const float titlebarHeight = 40.0f;
		ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		ImVec2 titlebarMax = ImVec2(
			titlebarMin.x + ImGui::GetWindowWidth(),
			titlebarMin.y + titlebarHeight
		);

		// Draw titlebar background behind menu bar
		ImGui::GetWindowDrawList()->AddRectFilled(titlebarMin, titlebarMax, IM_COL32(21, 21, 21, 255));

		// Title text on the left
		ImGui::SetCursorPos(ImVec2(10.0f, 3.0f));
		ImGui::TextUnformatted("HazelX Editor");

		// Standard menu bar (ImGuiWindowFlags_MenuBar is set on this window)
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene")) {}
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
				{
					auto& app = Application::Get();
					std::string filepath = app.OpenFile("Hazel Scene (*.hsc)\0*.hsc\0");
					if (!filepath.empty())
					{
						Ref<Scene> newScene = Ref<Scene>::Create();
						SceneSerializer serializer(newScene);
						serializer.Deserialize(filepath);
						m_EditorScene = newScene;
						m_SceneHierarchyPanel->SetContext(m_EditorScene);
						ScriptEngine::SetSceneContext(m_EditorScene);
						m_EditorScene->SetSelectedEntity({});
						m_SelectionContext.clear();
					}
				}
				if (ImGui::MenuItem("Save Scene...", "Ctrl+S"))
				{
					auto& app = Application::Get();
					std::string filepath = app.SaveFile("Hazel Scene (*.hsc)\0*.hsc\0");
					SceneSerializer serializer(m_EditorScene);
					serializer.Serialize(filepath);
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Reload C# Assembly"))
					ScriptEngine::ReloadAssembly("assets/scripts/ExampleApp.dll");
				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
					Application::Get().GetWindow().Close();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Toggle Grid", "Ctrl+G"))
					SceneRenderer::GetOptions().ShowGrid = !SceneRenderer::GetOptions().ShowGrid;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::PopStyleColor();

		// Window buttons - absolute positioned on the right side of the titlebar
		const float btnW = 46.0f;
		const float btnH = titlebarHeight - 2.0f;
		float btnX = ImGui::GetWindowWidth() - btnW * 3;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.3f));

		ImGui::SetCursorPos(ImVec2(btnX, 1.0f));
		if (ImGui::InvisibleButton("##min", ImVec2(btnW, btnH)))
			Application::Get().GetWindow().Minimize();

		btnX += btnW;
		ImGui::SetCursorPos(ImVec2(btnX, 1.0f));
		if (ImGui::InvisibleButton("##max", ImVec2(btnW, btnH)))
		{
			auto& win = Application::Get().GetWindow();
			if (win.IsMaximized()) win.Restore();
			else win.Maximize();
		}

		btnX += btnW;
		ImGui::SetCursorPos(ImVec2(btnX, 1.0f));
		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.15f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
		if (ImGui::InvisibleButton("##close", ImVec2(btnW, btnH)))
			Application::Get().GetWindow().Close();
		ImGui::PopStyleColor(3);

		// Drag: track state so dragging continues even if mouse leaves titlebar
		{
			static bool s_Dragging = false;
			static float s_OffsetX = 0, s_OffsetY = 0;

			ImVec2 mouse = ImGui::GetMousePos();
			bool inTitlebar = mouse.x >= titlebarMin.x && mouse.x < titlebarMax.x
				&& mouse.y >= titlebarMin.y && mouse.y < titlebarMax.y;

			// Start drag on click in empty titlebar area
			if (inTitlebar && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				s_Dragging = true;
				auto pos = Application::Get().GetWindow().GetWindowPos();
				s_OffsetX = mouse.x - pos.first;
				s_OffsetY = mouse.y - pos.second;
			}

			// Double-click toggles maximize
			if (inTitlebar && !ImGui::IsAnyItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				auto& win = Application::Get().GetWindow();
				if (win.IsMaximized()) win.Restore();
				else win.Maximize();
				s_Dragging = false;
			}

			// Continue drag regardless of mouse position
			if (s_Dragging)
			{
				auto& win = Application::Get().GetWindow();
				if (win.IsMaximized())
				{
					win.Restore();
					float ratio = (float)win.GetWidth() / (titlebarMax.x - titlebarMin.x);
					s_OffsetX *= ratio;
				}
				win.SetWindowPos((int)(mouse.x - s_OffsetX), (int)(mouse.y - s_OffsetY));

				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
					s_Dragging = false;
			}
		}

		ImGuiID dockspaceId = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f));

		// === 编辑器面板（这些是 Dockable 窗口，会被 DockSpace 管理） ===

		// Environment Panel
		ImGui::Begin("Environment");

		if (ImGui::Button("Load Environment Map"))
		{
			std::string filename = Application::Get().OpenFile("*.hdr");
			if (filename != "")
				m_EditorScene->SetEnvironment(Environment::Load(filename));
		}

		ImGui::SliderFloat("Skybox LOD", &m_EditorScene->GetSkyboxLod(), 0.0f, 11.0f);

		ImGui::Columns(2);
		ImGui::AlignTextToFramePadding();

		auto& light = m_EditorScene->GetLight();
		Property("Light Direction", light.Direction);
		Property("Light Radiance", light.Radiance, PropertyFlag::ColorProperty);
		Property("Light Multiplier", light.Multiplier, 0.0f, 5.0f);

		Property("Exposure", m_EditorCamera.GetExposure(), 0.0f, 5.0f);

		Property("Radiance Prefiltering", m_RadiancePrefilter);
		Property("Env Map Rotation", m_EnvMapRotation, -360.0f, 360.0f);

		if (Property("Show Bounding Boxes", m_UIShowBoundingBoxes))
			ShowBoundingBoxes(m_UIShowBoundingBoxes, m_UIShowBoundingBoxesOnTop);
		if (m_UIShowBoundingBoxes && Property("On Top", m_UIShowBoundingBoxesOnTop))
			ShowBoundingBoxes(m_UIShowBoundingBoxes, m_UIShowBoundingBoxesOnTop);

		char* label = m_SelectionMode == SelectionMode::Entity ? "Entity" : "Mesh";
		if (ImGui::Button(label))
		{
			m_SelectionMode = m_SelectionMode == SelectionMode::Entity ? SelectionMode::SubMesh : SelectionMode::Entity;
		}

		ImGui::Columns(1);

		ImGui::End();

		// Model / Material Panel
		ImGui::Begin("Model");

		ImGui::Separator();
		{
			ImGui::Text("Mesh");
		}
		ImGui::Separator();

		// Albedo
		if (ImGui::CollapsingHeader("Albedo", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(m_AlbedoInput.TextureMap ? (void*)m_AlbedoInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID(), ImVec2(64, 64));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (m_AlbedoInput.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(m_AlbedoInput.TextureMap->GetPath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image((void*)m_AlbedoInput.TextureMap->GetRendererID(), ImVec2(384, 384));
					ImGui::EndTooltip();
				}
				if (ImGui::IsItemClicked())
				{
					std::string filename = Application::Get().OpenFile("");
					if (filename != "")
						m_AlbedoInput.TextureMap = Texture2D::Create(filename, m_AlbedoInput.SRGB);
				}
			}
			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Checkbox("Use##AlbedoMap", &m_AlbedoInput.UseTexture);
			if (ImGui::Checkbox("sRGB##AlbedoMap", &m_AlbedoInput.SRGB))
			{
				if (m_AlbedoInput.TextureMap)
					m_AlbedoInput.TextureMap = Texture2D::Create(m_AlbedoInput.TextureMap->GetPath(), m_AlbedoInput.SRGB);
			}
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(m_AlbedoInput.Color), ImGuiColorEditFlags_NoInputs);
		}

		// Normals
		if (ImGui::CollapsingHeader("Normals", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(m_NormalInput.TextureMap ? (void*)m_NormalInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID(), ImVec2(64, 64));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (m_NormalInput.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(m_NormalInput.TextureMap->GetPath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image((void*)m_NormalInput.TextureMap->GetRendererID(), ImVec2(384, 384));
					ImGui::EndTooltip();
				}
				if (ImGui::IsItemClicked())
				{
					std::string filename = Application::Get().OpenFile("");
					if (filename != "")
						m_NormalInput.TextureMap = Texture2D::Create(filename);
				}
			}
			ImGui::SameLine();
			ImGui::Checkbox("Use##NormalMap", &m_NormalInput.UseTexture);
		}

		// Metalness
		if (ImGui::CollapsingHeader("Metalness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(m_MetalnessInput.TextureMap ? (void*)m_MetalnessInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID(), ImVec2(64, 64));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (m_MetalnessInput.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(m_MetalnessInput.TextureMap->GetPath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image((void*)m_MetalnessInput.TextureMap->GetRendererID(), ImVec2(384, 384));
					ImGui::EndTooltip();
				}
				if (ImGui::IsItemClicked())
				{
					std::string filename = Application::Get().OpenFile("");
					if (filename != "")
						m_MetalnessInput.TextureMap = Texture2D::Create(filename);
				}
			}
			ImGui::SameLine();
			ImGui::Checkbox("Use##MetalnessMap", &m_MetalnessInput.UseTexture);
			ImGui::SameLine();
			ImGui::SliderFloat("Value##MetalnessInput", &m_MetalnessInput.Value, 0.0f, 1.0f);
		}

		// Roughness
		if (ImGui::CollapsingHeader("Roughness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(m_RoughnessInput.TextureMap ? (void*)m_RoughnessInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID(), ImVec2(64, 64));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (m_RoughnessInput.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(m_RoughnessInput.TextureMap->GetPath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image((void*)m_RoughnessInput.TextureMap->GetRendererID(), ImVec2(384, 384));
					ImGui::EndTooltip();
				}
				if (ImGui::IsItemClicked())
				{
					std::string filename = Application::Get().OpenFile("");
					if (filename != "")
						m_RoughnessInput.TextureMap = Texture2D::Create(filename);
				}
			}
			ImGui::SameLine();
			ImGui::Checkbox("Use##RoughnessMap", &m_RoughnessInput.UseTexture);
			ImGui::SameLine();
			ImGui::SliderFloat("Value##RoughnessInput", &m_RoughnessInput.Value, 0.0f, 1.0f);
		}

		ImGui::Separator();

		if (ImGui::TreeNode("Shaders"))
		{
			auto& shaders = Shader::s_AllShaders;
			for (auto& shader : shaders)
			{
				if (ImGui::TreeNode(shader->GetName().c_str()))
				{
					std::string buttonName = "Reload##" + shader->GetName();
					if (ImGui::Button(buttonName.c_str()))
						shader->Reload();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		ImGui::End();

		// Toolbar
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::Begin("Toolbar");
		if (m_SceneState == SceneState::Edit)
		{
			if (ImGui::ImageButton("PlayButton", (ImTextureID)m_PlayButtonTex->GetRendererID(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(0.9f, 0.9f, 0.9f, 1.0f)))
			{
				OnScenePlay();
			}
		}
		else if (m_SceneState == SceneState::Play)
		{
			if (ImGui::ImageButton("StopButton", (ImTextureID)m_PlayButtonTex->GetRendererID(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 0.2f)))
			{
				OnSceneStop();
			}
		}
		ImGui::SameLine();
		if (ImGui::ImageButton("PlayButtonExtra", (ImTextureID)m_PlayButtonTex->GetRendererID(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1.0f, 1.0f, 1.0f, 0.6f)))
		{
			HZ_CORE_INFO("PLAY!");
		}
		ImGui::End();
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(3);

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		m_ViewportPanelMouseOver = ImGui::IsWindowHovered();
		m_ViewportPanelFocused = ImGui::IsWindowFocused();

		auto viewportOffset = ImGui::GetCursorPos();
		auto viewportSize = ImGui::GetContentRegionAvail();
		SceneRenderer::SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_EditorScene->SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		if (m_RuntimeScene)
			m_RuntimeScene->SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_EditorCamera.SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 10000.0f));
		m_EditorCamera.SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		ImGui::Image((void*)SceneRenderer::GetFinalColorBufferRendererID(), viewportSize, { 0, 1 }, { 1, 0 });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };
		m_AllowViewportCameraEvents = ImGui::IsMouseHoveringRect(minBound, maxBound);

		// Gizmos
		if (m_GizmoType != -1 && m_SelectionContext.size())
		{
			auto& selection = m_SelectionContext[0];

			float rw = (float)ImGui::GetWindowWidth();
			float rh = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);

			bool snap = Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL);

			auto& entityTransform = selection.Entity.Transform();
			float snapValue[3] = { m_SnapValue, m_SnapValue, m_SnapValue };
			if (m_SelectionMode == SelectionMode::Entity)
			{
				ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.GetViewMatrix()),
					glm::value_ptr(m_EditorCamera.GetProjectionMatrix()),
					(ImGuizmo::OPERATION)m_GizmoType,
					ImGuizmo::LOCAL,
					glm::value_ptr(entityTransform),
					nullptr,
					snap ? snapValue : nullptr);
			}
			else
			{
				glm::mat4 transformBase = entityTransform * selection.Mesh->Transform;
				ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.GetViewMatrix()),
					glm::value_ptr(m_EditorCamera.GetProjectionMatrix()),
					(ImGuizmo::OPERATION)m_GizmoType,
					ImGuizmo::LOCAL,
					glm::value_ptr(transformBase),
					nullptr,
					snap ? snapValue : nullptr);

				selection.Mesh->Transform = glm::inverse(entityTransform) * transformBase;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		// Scene Hierarchy & Script Debug
		m_SceneHierarchyPanel->OnImGuiRender();
		ScriptEngine::OnImGuiRender();

		ImGui::End(); // DockSpace window
	}

	void EditorLayer::OnEvent(Event& e)
	{
		if (m_SceneState == SceneState::Edit)
		{
			if (m_ViewportPanelMouseOver)
				m_EditorCamera.OnEvent(e);
			m_EditorScene->OnEvent(e);
		}
		else if (m_SceneState == SceneState::Play)
		{
			m_RuntimeScene->OnEvent(e);
		}

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (m_ViewportPanelFocused)
		{
			switch (e.GetKeyCode())
			{
				case KeyCode::Q:
					m_GizmoType = -1;
					break;
				case KeyCode::W:
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
					break;
				case KeyCode::E:
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
					break;
				case KeyCode::R:
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
					break;
				case KeyCode::Delete:
					if (m_SelectionContext.size())
					{
						Entity selectedEntity = m_SelectionContext[0].Entity;
						m_EditorScene->DestroyEntity(selectedEntity);
						m_SelectionContext.clear();
						m_EditorScene->SetSelectedEntity({});
						m_SceneHierarchyPanel->SetSelected({});
					}
					break;
			}
		}

		if (Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL))
		{
			switch (e.GetKeyCode())
			{
				case KeyCode::G:
					SceneRenderer::GetOptions().ShowGrid = !SceneRenderer::GetOptions().ShowGrid;
					break;
				case KeyCode::B:
					m_UIShowBoundingBoxes = !m_UIShowBoundingBoxes;
					ShowBoundingBoxes(m_UIShowBoundingBoxes, m_UIShowBoundingBoxesOnTop);
					break;
				case KeyCode::D:
					if (m_SelectionContext.size())
					{
						Entity selectedEntity = m_SelectionContext[0].Entity;
						m_EditorScene->DuplicateEntity(selectedEntity);
					}
					break;
			}
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		auto [mx, my] = Input::GetMousePosition();
		if (e.GetMouseButton() == HZ_MOUSE_BUTTON_LEFT && !Input::IsKeyPressed(KeyCode::LeftAlt) && !ImGuizmo::IsOver())
		{
			auto [mouseX, mouseY] = GetMouseViewportSpace();
			if (mouseX > -1.0f && mouseX < 1.0f && mouseY > -1.0f && mouseY < 1.0f)
			{
				auto [origin, direction] = CastRay(mouseX, mouseY);

				m_SelectionContext.clear();
				m_EditorScene->SetSelectedEntity({});
				auto meshEntities = m_EditorScene->GetAllEntitiesWith<MeshComponent>();
				for (auto e : meshEntities)
				{
					Entity entity = { e, m_EditorScene.Raw() };
					auto mesh = entity.GetComponent<MeshComponent>().Mesh;
					if (!mesh)
						continue;

					auto& submeshes = mesh->GetSubmeshes();
					float lastT = std::numeric_limits<float>::max();
					for (uint32_t i = 0; i < submeshes.size(); i++)
					{
						auto& submesh = submeshes[i];
						Ray ray = {
							glm::inverse(entity.Transform() * submesh.Transform) * glm::vec4(origin, 1.0f),
							glm::inverse(glm::mat3(entity.Transform()) * glm::mat3(submesh.Transform)) * direction
						};

						float t;
						bool intersects = ray.IntersectsAABB(submesh.BoundingBox, t);
						if (intersects)
						{
							const auto& triangleCache = mesh->GetTriangleCache(i);
							for (const auto& triangle : triangleCache)
							{
								if (ray.IntersectsTriangle(triangle.V0.Position, triangle.V1.Position, triangle.V2.Position, t))
								{
									HZ_WARN("INTERSECTION: {0}, t={1}", submesh.NodeName, t);
									m_SelectionContext.push_back({ entity, &submesh, t });
									break;
								}
							}
						}
					}
				}
				std::sort(m_SelectionContext.begin(), m_SelectionContext.end(), [](auto& a, auto& b) { return a.Distance < b.Distance; });
				if (m_SelectionContext.size())
					OnSelected(m_SelectionContext[0]);
			}
		}
		return false;
	}

	std::pair<float, float> EditorLayer::GetMouseViewportSpace()
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
		auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;
		return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}

	std::pair<glm::vec3, glm::vec3> EditorLayer::CastRay(float mx, float my)
	{
		glm::vec4 mouseClipPos = { mx, my, -1.0f, 1.0f };

		auto inverseProj = glm::inverse(m_EditorCamera.GetProjectionMatrix());
		auto inverseView = glm::inverse(glm::mat3(m_EditorCamera.GetViewMatrix()));
		glm::vec4 ray = inverseProj * mouseClipPos;
		glm::vec3 rayPos = m_EditorCamera.GetPosition();
		glm::vec3 rayDir = inverseView * glm::vec3(ray);

		return { rayPos, rayDir };
	}

	void EditorLayer::OnSelected(const SelectedSubmesh& selectionContext)
	{
		m_SceneHierarchyPanel->SetSelected(selectionContext.Entity);
		m_EditorScene->SetSelectedEntity(selectionContext.Entity);
	}

	void EditorLayer::OnEntityDeleted(Entity e)
	{
		if (m_SelectionContext[0].Entity == e)
		{
			m_SelectionContext.clear();
			m_EditorScene->SetSelectedEntity({});
		}
	}

	Ray EditorLayer::CastMouseRay()
	{
		auto [mouseX, mouseY] = GetMouseViewportSpace();
		if (mouseX > -1.0f && mouseX < 1.0f && mouseY > -1.0f && mouseY < 1.0f)
		{
			auto [origin, direction] = CastRay(mouseX, mouseY);
			return Ray(origin, direction);
		}
		return Ray::Zero();
	}
}
