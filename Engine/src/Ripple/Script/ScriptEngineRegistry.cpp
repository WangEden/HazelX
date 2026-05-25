#include "hzpch.h"
#include "ScriptEngineRegistry.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Ripple/Scene/Entity.h"
#include "ScriptWrappers.h"
#include <iostream>

namespace Ripple {

	std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;

	extern MonoImage* s_CoreAssemblyImage;

#define Component_RegisterType(Type) \
	{\
		MonoType* type = mono_reflection_type_from_name("Ripple." #Type, s_CoreAssemblyImage);\
		if (type) {\
			uint32_t id = mono_type_get_type(type);\
			s_HasComponentFuncs[type] = [](Entity& entity) { return entity.HasComponent<Type>(); };\
			s_CreateComponentFuncs[type] = [](Entity& entity) { entity.AddComponent<Type>(); };\
		} else {\
			RP_CORE_ERROR("No C# component class found for " #Type "!");\
		}\
	}

	static void InitComponentTypes()
	{
		Component_RegisterType(TagComponent);
		Component_RegisterType(TransformComponent);
		Component_RegisterType(MeshComponent);
		Component_RegisterType(ScriptComponent);
		Component_RegisterType(CameraComponent);
		Component_RegisterType(SpriteRendererComponent);
	}

	void ScriptEngineRegistry::RegisterAll()
	{
		InitComponentTypes();

		mono_add_internal_call("Ripple.Noise::PerlinNoise_Native", Ripple::Script::Ripple_Noise_PerlinNoise);

		mono_add_internal_call("Ripple.Entity::GetTransform_Native", Ripple::Script::Ripple_Entity_GetTransform);
		mono_add_internal_call("Ripple.Entity::SetTransform_Native", Ripple::Script::Ripple_Entity_SetTransform);
		mono_add_internal_call("Ripple.Entity::CreateComponent_Native", Ripple::Script::Ripple_Entity_CreateComponent);
		mono_add_internal_call("Ripple.Entity::HasComponent_Native", Ripple::Script::Ripple_Entity_HasComponent);

		mono_add_internal_call("Ripple.MeshComponent::GetMesh_Native", Ripple::Script::Ripple_MeshComponent_GetMesh);
		mono_add_internal_call("Ripple.MeshComponent::SetMesh_Native", Ripple::Script::Ripple_MeshComponent_SetMesh);

		mono_add_internal_call("Ripple.Input::IsKeyPressed_Native", Ripple::Script::Ripple_Input_IsKeyPressed);

		mono_add_internal_call("Ripple.Texture2D::Constructor_Native", Ripple::Script::Ripple_Texture2D_Constructor);
		mono_add_internal_call("Ripple.Texture2D::Destructor_Native", Ripple::Script::Ripple_Texture2D_Destructor);
		mono_add_internal_call("Ripple.Texture2D::SetData_Native", Ripple::Script::Ripple_Texture2D_SetData);

		mono_add_internal_call("Ripple.Material::Destructor_Native", Ripple::Script::Ripple_Material_Destructor);
		mono_add_internal_call("Ripple.Material::SetFloat_Native", Ripple::Script::Ripple_Material_SetFloat);
		mono_add_internal_call("Ripple.Material::SetTexture_Native", Ripple::Script::Ripple_Material_SetTexture);

		mono_add_internal_call("Ripple.MaterialInstance::Destructor_Native", Ripple::Script::Ripple_MaterialInstance_Destructor);
		mono_add_internal_call("Ripple.MaterialInstance::SetFloat_Native", Ripple::Script::Ripple_MaterialInstance_SetFloat);
		mono_add_internal_call("Ripple.MaterialInstance::SetVector3_Native", Ripple::Script::Ripple_MaterialInstance_SetVector3);
		mono_add_internal_call("Ripple.MaterialInstance::SetTexture_Native", Ripple::Script::Ripple_MaterialInstance_SetTexture);

		mono_add_internal_call("Ripple.Mesh::Constructor_Native", Ripple::Script::Ripple_Mesh_Constructor);
		mono_add_internal_call("Ripple.Mesh::Destructor_Native", Ripple::Script::Ripple_Mesh_Destructor);
		mono_add_internal_call("Ripple.Mesh::GetMaterial_Native", Ripple::Script::Ripple_Mesh_GetMaterial);
		mono_add_internal_call("Ripple.Mesh::GetMaterialByIndex_Native", Ripple::Script::Ripple_Mesh_GetMaterialByIndex);
		mono_add_internal_call("Ripple.Mesh::GetMaterialCount_Native", Ripple::Script::Ripple_Mesh_GetMaterialCount);

		mono_add_internal_call("Ripple.MeshFactory::CreatePlane_Native", Ripple::Script::Ripple_MeshFactory_CreatePlane);

		// static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }
		// 
		// static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		// static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		// static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		// static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	}



}
