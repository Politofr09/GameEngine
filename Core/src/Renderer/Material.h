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

		static AssetHandle Create(const std::string& path, const std::string& name);

		bool Load();
		bool UnLoad() override { return true; }

		Material() = default;

		// Metadata constructor for (de)serializing + texture references
		Material(const AssetMetadata& metadata, AssetHandle diffuseTexture, AssetHandle specularTexture, AssetHandle normalTexture);

		Material& operator=(const Material& other)
		{
			this->m_ShaderType = other.m_ShaderType;
			this->Color = other.Color;
			this->Ambient = other.Ambient;
			this->Diffuse = other.Diffuse;
			this->Specular = other.Specular;
			this->Shininess = other.Shininess;

			this->DiffuseTextureHandle = other.DiffuseTextureHandle;
			this->SpecularTextureHandle = other.SpecularTextureHandle;
			this->NormalTextureHandle = other.NormalTextureHandle;

			return *this;
		}

		// This let's the renderer know what Shader to bind when rendering the object
		ShaderType m_ShaderType = ShaderType::None;
		aiMaterial* m_AssimpSrcMaterial = nullptr;

		AssetHandle DiffuseTextureHandle = 0;
		AssetHandle SpecularTextureHandle = 0;
		AssetHandle NormalTextureHandle = 0;

		glm::vec3 Color = glm::vec3(0.0f);
		glm::vec3 Ambient = glm::vec3(0.0f);
		glm::vec3 Diffuse = glm::vec3(0.0f);
		glm::vec3 Specular = glm::vec3(0.0f);
		float Shininess = 0.0f;
	};

}