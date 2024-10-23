#include "Core/Asset.h"
#include "Core/AssetRegistry.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#pragma once

namespace Core::Gfx
{

	enum class ShaderType
	{
		None,
		FlatShading,
		PhongShading
		// TODO: Add more types as we develop more rendering techniques and shaders
	};

	class Shader : public Asset
	{
	public:
		DECLARE_ASSET_TYPE("Shader")

		static Shader& Create(AssetRegistry& registry, const std::string& path, const std::string& name);

		// Default constructor
		Shader() = default;

		// Metadata constructor (for (de)serializing)
		Shader(const AssetMetadata& metadata);

		bool Load() override;
		bool UnLoad() override;
		bool LoadFromMemory(const char* vertexShaderSource, const char* fragmentShaderSource);

		void Use();
		void End();

		unsigned int GetID() { return m_RendererID; }

		// Uniform loading functions
		void SetBool(const std::string& uniformName, bool value);
		void SetFloat(const std::string& uniformName, float value);
		void SetInt(const std::string& uniformName, int value);
		void SetVector2(const std::string& uniformName, glm::vec2 value);
		void SetVector3(const std::string& uniformName, glm::vec3 value);
		void SetVector4(const std::string& uniformName, glm::vec4 value);
		void SetMatrix(const std::string& uniformName, glm::mat4 value);
	

	private:
		unsigned int m_RendererID = 0;
	};

}