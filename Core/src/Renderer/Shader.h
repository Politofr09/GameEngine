#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Core::Gfx
{

	class Shader
	{
	public:
		Shader() = default;
		Shader(const char* vertexPath, const char* fragmentPath);
		Shader& operator=(const Shader& other)
		{
			this->ID = other.ID;
			return *this;
		}

		void Load(const char* vertexPath, const char* fragmentPath);
		void LoadFromMemory(const char* vertexSource, const char* fragmentSource);

		void Use();
		void End();

		unsigned int GetID() { return ID; }

		// Uniform loading functions
		void SetBool(const std::string& uniformName, bool value);
		void SetFloat(const std::string& uniformName, float value);
		void SetInt(const std::string& uniformName, int value);
		void SetVector2(const std::string& uniformName, glm::vec2 value);
		void SetVector3(const std::string& uniformName, glm::vec3 value);
		void SetVector4(const std::string& uniformName, glm::vec4 value);
		void SetMatrix(const std::string& uniformName, glm::mat4 value);

	private:
		GLuint ID;
	};

}