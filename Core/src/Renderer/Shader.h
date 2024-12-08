#include "Core/Asset.h"

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

	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& path);
		static Ref<Shader> Create(const AssetMetadata& metadata);

		// Default constructor
		Shader() = default;
		Shader(const AssetMetadata& metadata)
			: m_Metadata(metadata) {}

		bool Load();
		bool UnLoad();
		bool LoadFromMemory(const char* vertexShaderSource, const char* fragmentShaderSource);

		void Use() const;
		void End() const;

		unsigned int GetID() const { return m_RendererID; }

		void SetBool(const std::string& uniformName, bool value);
		void SetFloat(const std::string& uniformName, float value);
		void SetInt(const std::string& uniformName, int value);
		void SetVector2(const std::string& uniformName, glm::vec2 value);
		void SetVector3(const std::string& uniformName, glm::vec3 value);
		void SetVector4(const std::string& uniformName, glm::vec4 value);
		void SetMatrix(const std::string& uniformName, glm::mat4 value);
	
		AssetMetadata& GetMetadata() { return m_Metadata; }
		bool IsLoaded() const { return m_Loaded; }

	private:
		unsigned int m_RendererID = 0;
		AssetMetadata m_Metadata;
		bool m_Loaded = false;
	};

}