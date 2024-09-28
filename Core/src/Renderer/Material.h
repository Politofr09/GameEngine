#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Core/Asset.h"

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/material.h>

namespace Core::Gfx
{

	class Material : public Asset
	{
	public:
		DECLARE_ASSET_TYPE("Material")

		static Material& CreateFromAssimp(AssetRegistry& registry, aiMaterial* mat, const std::string& directory);

		bool Load() override { return true; }
		bool UnLoad() override { return true; }

		Material() = default;
		Material& operator=(const Material& other)
		{
			this->m_ShaderType = other.m_ShaderType;
			this->Color = other.Color;
			this->Ambient = other.Ambient;
			this->Diffuse = other.Diffuse;
			this->Specular = other.Specular;
			this->Shininess = other.Shininess;

			if (other.DiffuseTexture.IsLoaded())
				this->DiffuseTexture = other.DiffuseTexture;

			if (other.SpecularTexture.IsLoaded())
				this->SpecularTexture = other.SpecularTexture;

			if (other.NormalTexture.IsLoaded())
				this->NormalTexture = other.NormalTexture;

			return *this;
		}

		// This field let's the renderer know what Shader to bind when rendering the object
		ShaderType m_ShaderType = ShaderType::None;

		// Common textures
		Texture DiffuseTexture;
		Texture SpecularTexture;
		Texture NormalTexture;

		// Common atributes
		glm::vec3 Color = glm::vec3(0.0f);
		glm::vec3 Ambient = glm::vec3(0.0f);
		glm::vec3 Diffuse = glm::vec3(0.0f);
		glm::vec3 Specular = glm::vec3(0.0f);
		float Shininess = 0.0f;
	};

}