#pragma once

#include "Ripple/Script/ScriptEngine.h"
#include "Ripple/Core/KeyCodes.h"

#include <glm/glm.hpp>

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
}

namespace Ripple {
	namespace Script {
		// Math
		float Ripple_Noise_PerlinNoise(float x, float y);

		// Input
		bool Ripple_Input_IsKeyPressed(KeyCode key);

		// Entity
		void Ripple_Entity_GetTransform(uint64_t entityID, glm::mat4* outTransform);
		void Ripple_Entity_SetTransform(uint64_t entityID, glm::mat4* inTransform);
		void Ripple_Entity_CreateComponent(uint64_t entityID, void* type);
		bool Ripple_Entity_HasComponent(uint64_t entityID, void* type);

		void* Ripple_MeshComponent_GetMesh(uint64_t entityID);
		void Ripple_MeshComponent_SetMesh(uint64_t entityID, Ref<Mesh>* inMesh);

		// Renderer
		// Texture2D
		void* Ripple_Texture2D_Constructor(uint32_t width, uint32_t height);
		void Ripple_Texture2D_Destructor(Ref<Texture2D>* _this);
		void Ripple_Texture2D_SetData(Ref<Texture2D>* _this, MonoArray* inData, int32_t count);

		// Material
		void Ripple_Material_Destructor(Ref<Material>* _this);
		void Ripple_Material_SetFloat(Ref<Material>* _this, MonoString* uniform, float value);
		void Ripple_Material_SetTexture(Ref<Material>* _this, MonoString* uniform, Ref<Texture2D>* texture);

		void Ripple_MaterialInstance_Destructor(Ref<MaterialInstance>* _this);
		void Ripple_MaterialInstance_SetFloat(Ref<MaterialInstance>* _this, MonoString* uniform, float value);
		void Ripple_MaterialInstance_SetVector3(Ref<MaterialInstance>* _this, MonoString* uniform, glm::vec3* value);
		void Ripple_MaterialInstance_SetTexture(Ref<MaterialInstance>* _this, MonoString* uniform, Ref<Texture2D>* texture);

		// Mesh
		Ref<Mesh>* Ripple_Mesh_Constructor(MonoString* filepath);
		void Ripple_Mesh_Destructor(Ref<Mesh>* _this);
		Ref<Material>* Ripple_Mesh_GetMaterial(Ref<Mesh>* inMesh);
		Ref<MaterialInstance>* Ripple_Mesh_GetMaterialByIndex(Ref<Mesh>* inMesh, int index);
		int Ripple_Mesh_GetMaterialCount(Ref<Mesh>* inMesh);

		void* Ripple_MeshFactory_CreatePlane(float width, float height);
	}
}
