#include "Core/Resource.h"
#include "Core/ResourceManager.h"

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

	class Shader : public Resource
	{
	public:
		DECLARE_RESOURCE_TYPE("Shader")

		static Shader& Create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& name);

		Shader() = default;
		Shader& operator=(const Shader& other)
		{
			this->m_RendererID = other.m_RendererID;
			return *this;
		}

		bool Load() override;
		bool UnLoad() override;
		bool LoadFromMemory(const char* vertexSource, const char* fragmentSource);

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
		Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& name);
		std::string m_VertexPath;
		std::string m_FragmentPath;

	private:
		unsigned int m_RendererID;
	};

}