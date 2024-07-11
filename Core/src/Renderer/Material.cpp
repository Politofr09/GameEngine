#include "Material.h"

namespace Core::Gfx
{

	Material::Material(const Shader& shader, const Texture& texture)
	{
		m_Shader = shader;
		m_Texture = texture;
	}

	void Material::UpdateUniformValues()
	{
		m_Shader.Use();
		for (auto& pair : m_UniformInts)
			m_Shader.SetInt(pair.first, pair.second);

		for (auto& pair : m_UniformFloats)
			m_Shader.SetFloat(pair.first, pair.second);

		for (auto& pair : m_UniformFloat2s)
			m_Shader.SetVector2(pair.first, pair.second);

		for (auto& pair : m_UniformFloat3s)
			m_Shader.SetVector3(pair.first, pair.second);

		for (auto& pair : m_UniformFloat4s)
			m_Shader.SetVector4(pair.first, pair.second);

		for (auto& pair : m_UniformMat4s)
			m_Shader.SetMatrix(pair.first, pair.second);
	}

	void Material::SetInt(const std::string& name, int value)
	{
		m_UniformInts[name] = value;
	}

	void Material::SetFloat(const std::string& name, float value)
	{
		m_UniformFloats[name] = value;
	}

	void Material::SetVector2(const std::string& name, glm::vec2 value)
	{
		m_UniformFloat2s[name] = value;
	}

	void Material::SetVector3(const std::string& name, glm::vec3 value)
	{
		m_UniformFloat3s[name] = value;
	}

	void Material::SetVector4(const std::string& name, glm::vec4 value)
	{
		m_UniformFloat4s[name] = value;
	}

	void Material::SetMatrix(const std::string& name, glm::mat4 value)
	{
		m_UniformMat4s[name] = value;
	}

	int Material::GetIntUniform(const std::string& name)
	{
		return m_UniformInts[name];
	}

	float Material::GetFloatUniform(const std::string& name)
	{
		return m_UniformFloats[name];
	}

	glm::vec2 Material::GetVec2Uniform(const std::string& name)
	{
		return m_UniformFloat2s[name];
	}

	glm::vec3 Material::GetVec3Uniform(const std::string& name)
	{
		return m_UniformFloat3s[name];
	}

	glm::vec4 Material::GetVec4Uniform(const std::string& name)
	{
		return m_UniformFloat4s[name];
	}

	glm::mat4 Material::GetMatrixUniform(const std::string& name)
	{
		return m_UniformMat4s[name];
	}

}