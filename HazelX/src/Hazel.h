#pragma once

/**
 * 这是给应用程序使用的头文件
 */
#include "Hazel/Core/Application.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Core/Events/Event.h"
#include "Hazel/Core/Events/ApplicationEvent.h"
#include "Hazel/Core/Events/KeyEvent.h"
#include "Hazel/Core/Events/MouseEvent.h"

#include "imgui/imgui.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"

// ---------- Renderer ------------
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Renderer/RenderPass.h"
#include "Hazel/Renderer/Framebuffer.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Mesh.h"
#include "Hazel/Renderer/Camera.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/Material.h"
// --------------------------------

// ---------- Physics ------------
#include "Hazel/Physics2D/Physics2D.h"
// --------------------------------
