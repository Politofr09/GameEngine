#pragma once

#include "Texture.h"
#include "Shader.h"

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace Core::Gfx
{
	
	class Material
	{
	public:
		Material() = default;
		Material& operator=(const Material& other)
		{
			m_Shader = other.m_Shader;
			m_Texture = other.m_Texture;
			/*if (!other.m_UniformInts.empty())	m_UniformInts = other.m_UniformInts;
			if (!other.m_UniformFloats.empty()) m_UniformFloats = other.m_UniformFloats;
			if (!other.m_UniformFloat2s.empty()) m_UniformFloat2s = other.m_UniformFloat2s;
			if (!other.m_UniformFloat3s.empty()) m_UniformFloat3s = other.m_UniformFloat3s;
			if (!other.m_UniformFloat4s.empty()) m_UniformFloat4s = other.m_UniformFloat4s;
			if (!other.m_UniformMat4s.empty())	m_UniformMat4s = other.m_UniformMat4s;
			*/
			return *this; 
		}
		Material(const Shader& shader, const Texture& texture);

		Shader& GetShader() { return m_Shader; }
		Texture& GetTexture() { return m_Texture; }

		void SetTexture(Texture texture) { m_Texture = texture; }
		void SetShader(Shader shader) { m_Shader = shader; }

		void UpdateUniformValues();

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetVector2(const std::string& name, glm::vec2 value);
		void SetVector3(const std::string& name, glm::vec3 value);
		void SetVector4(const std::string& name, glm::vec4 value);
		void SetMatrix(const std::string& name, glm::mat4 value);

		int GetIntUniform(const std::string& name);
		float GetFloatUniform(const std::string& name);
		glm::vec2 GetVec2Uniform(const std::string& name);
		glm::vec3 GetVec3Uniform(const std::string& name);
		glm::vec4 GetVec4Uniform(const std::string& name);
		glm::mat4 GetMatrixUniform(const std::string& name);

	private:
		Shader m_Shader;
		Texture m_Texture;

		// Data
		std::unordered_map<std::string, int> m_UniformInts;
		std::unordered_map<std::string, float> m_UniformFloats;
		std::unordered_map<std::string, glm::vec2> m_UniformFloat2s;
		std::unordered_map<std::string, glm::vec3> m_UniformFloat3s;
		std::unordered_map<std::string, glm::vec4> m_UniformFloat4s;
		std::unordered_map<std::string, glm::mat4> m_UniformMat4s;
	};

}