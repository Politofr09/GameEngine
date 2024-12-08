#pragma once

#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <assimp/material.h>

namespace Core::Gfx
{
	
	class Model;

	class Material
	{
	public:
		// Used when the engine defines custom materials (They are not directly paired to a model)
		// In that case we load material from ".material" file (YAML)
		static Ref<Material> Create(const std::string& path);
		void Deserialize();
		void Serialize() const;

		// Used when models come with their own material
		static Ref<Material> CreateFromMemory(aiMaterial* mat, const std::string& modelFileName);
		bool LoadFromMemory(aiMaterial* material, const std::string& modelFileName);

		bool UnLoad() { m_Loaded = false; return true; }

		Material() = default;
		Material(const AssetMetadata& metadata)
		{
			Path = metadata.Path;
			Name = metadata.Name;
		}

		Ref<Texture> DiffuseTexture  = nullptr;
		Ref<Texture> SpecularTexture = nullptr;
		Ref<Texture> NormalTexture   = nullptr;
		Ref<Shader>  Shader			 = nullptr;
	
		glm::vec3	 Color = glm::vec3(0.0f);
		glm::vec3	 Ambient = glm::vec3(0.0f);
		glm::vec3	 Diffuse = glm::vec3(0.0f);
		glm::vec3	 Specular = glm::vec3(0.0f);
		float		 Shininess = 0.0f;

		std::string  Path;
		std::string  Name;
		UUID		 ID;

		bool IsLoaded() const { return m_Loaded; }
	private:


	private:
		bool m_Loaded = false;
	};

}