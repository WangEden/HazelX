#pragma once

#include <glm/glm.hpp>

#include "Ripple/Core/UUID.h"
#include "Ripple/Renderer/Texture.h"
#include "Ripple/Renderer/Mesh.h"
#include "Ripple/Scene/SceneCamera.h"

namespace Ripple {

	struct IDComponent
	{
		UUID ID = 0;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent& other)
			: Tag(other.Tag) {
		}
		TagComponent(const std::string& tag)
			: Tag(tag) {
		}

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent // → 在使用时能伪装 glm::mat4
	{
		glm::mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other)
			: Transform(other.Transform) {
		}
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {
		}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct MeshComponent
	{
		Ref<Ripple::Mesh> Mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent& other)
			: Mesh(other.Mesh) {
		}
		MeshComponent(const Ref<Ripple::Mesh>& mesh)
			: Mesh(mesh) {
		}

		operator Ref<Ripple::Mesh>() { return Mesh; }
	};

	struct ScriptComponent
	{
		// TODO: C# script
		std::string ModuleName;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent& other)
			: ModuleName(other.ModuleName) {
		}
		ScriptComponent(const std::string& moduleName)
			: ModuleName(moduleName) {
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other)
			: Camera(other.Camera), Primary(other.Primary) {
		}

		operator SceneCamera& () { return Camera; }
		operator const SceneCamera& () const { return Camera; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other)
			: Color(other.Color), Texture(other.Texture), TilingFactor(other.TilingFactor) {
		}
	};
}
