#include "hzpch.h"
#include "Hazel/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace Hazel {

	static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				HZ_CORE_ERROR("[OpenGL Debug HIGH] {0}", message);
				HZ_CORE_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				HZ_CORE_WARN("[OpenGL Debug MEDIUM] {0}", message);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				HZ_CORE_INFO("[OpenGL Debug LOW] {0}", message);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				// HZ_CORE_TRACE("[OpenGL Debug NOTIFICATION] {0}", message);
				break;
		}
	}

	void RendererAPI::Init()
	{
		glDebugMessageCallback(OpenGLLogMessage, nullptr); // 注册一个回调函数来接收OpenGL的调试消息
		glEnable(GL_DEBUG_OUTPUT); // 开启调试输出
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // 开启SYNCHRONOUS 确保错误发生时立即调用回调函数

		// TODO：
		// 当前只是一个单网格的渲染场景Demo，后续需要改成一个网格对应一个VAO的模式
		unsigned int vao; // 核心配置文件下，需要手动创建并绑定一个顶点数组对象（VAO），否则OpenGL会拒绝渲染任何东西
		glGenVertexArrays(1, &vao); 
		glBindVertexArray(vao);

		glEnable(GL_DEPTH_TEST); // 开启深度测试
		//glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glFrontFace(GL_CCW); // 规定逆时针排列的顶点为正面

		glEnable(GL_BLEND); // 开启混合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 标准的alpha混合函数，用于处理透明或半透明对象

		glEnable(GL_MULTISAMPLE);

		auto& caps = RendererAPI::GetCapabilities(); // 硬件能力探针，获取硬件参数

		caps.Vendor = (const char*)glGetString(GL_VENDOR);
		caps.Renderer = (const char*)glGetString(GL_RENDERER);
		caps.Version = (const char*)glGetString(GL_VERSION);

		glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples); // 获取最大多重采样数
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.MaxAnisotropy); // 获取最大各向异性过滤级别

		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &caps.MaxTextureUnits);

		GLenum error = glGetError();
		while (error != GL_NO_ERROR)
		{
			HZ_CORE_ERROR("OpenGL Error: {0}", error);
			error = glGetError();
		}

		LoadRequiredAssest();
	}

	void RendererAPI::Shutdown()
	{
	}

	void RendererAPI::LoadRequiredAssest()
	{
	}

	void RendererAPI::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void RendererAPI::DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest)
	{
		if (!depthTest)
			glDisable(GL_DEPTH_TEST);

		GLenum glPrimitiveType = 0;
		switch (type)
		{
		case PrimitiveType::Triangles:
			glPrimitiveType = GL_TRIANGLES;
			break;
		case PrimitiveType::Lines:
			glPrimitiveType = GL_LINES;
			break;
		}

		glDrawElements(glPrimitiveType, count, GL_UNSIGNED_INT, nullptr);

		if (!depthTest)
			glEnable(GL_DEPTH_TEST);
	}

	void RendererAPI::SetLineThickness(float thickness)
	{
		glLineWidth(thickness);
	}

}
