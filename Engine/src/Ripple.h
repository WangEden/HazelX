#pragma once

/**
 * 这是给应用程序使用的头文件
 */
#include "Ripple/Core/Application.h"
#include "Ripple/Core/Log.h"
#include "Ripple/Core/Input.h"
#include "Ripple/Core/Timestep.h"
#include "Ripple/Core/Timer.h"

#include "Ripple/Core/Events/Event.h"
#include "Ripple/Core/Events/ApplicationEvent.h"
#include "Ripple/Core/Events/KeyEvent.h"
#include "Ripple/Core/Events/MouseEvent.h"

#include "Ripple/Core/Math/AABB.h"
#include "Ripple/Core/Math/Ray.h"

#include "imgui/imgui.h"

#include "Ripple/Core/KeyCodes.h"
#include "Ripple/Core/MouseButtonCodes.h"

// ---------- Renderer ------------
#include "Ripple/Renderer/Renderer.h"
#include "Ripple/Renderer/SceneRenderer.h"
#include "Ripple/Renderer/RenderPass.h"
#include "Ripple/Renderer/Framebuffer.h"
#include "Ripple/Renderer/Buffer.h"
#include "Ripple/Renderer/VertexArray.h"
#include "Ripple/Renderer/Texture.h"
#include "Ripple/Renderer/Shader.h"
#include "Ripple/Renderer/Mesh.h"
#include "Ripple/Renderer/Camera.h"
#include "Ripple/Renderer/Material.h"

// --------------------------------

// Scenes
#include "Ripple/Scene/Entity.h"

#include "Ripple/Scene/Scene.h"
#include "Ripple/Scene/SceneCamera.h"
#include "Ripple/Scene/SceneSerializer.h"
#include "Ripple/Scene/Components.h"
