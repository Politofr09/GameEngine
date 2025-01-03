#include "Shader.h"
#include "Core/Utils.h"
#include "Core/Instrumentor.h"

namespace Core::Gfx
{

    Ref<Shader> Shader::Create(const std::string& path)
    {
        AssetMetadata metadata;
        metadata.Path = path;
        metadata.ID = UUID();
        metadata.Name = "Shader" + std::to_string(metadata.ID);
        metadata.Type = "Shader";
        return Shader::Create(metadata);
    }

    Ref<Shader> Shader::Create(const AssetMetadata& metadata)
    {
        Ref<Shader> shader = CreateRef<Shader>(metadata);
        shader->Load();
        return shader;
    }

    bool Shader::Load()
    {
        CORE_PROFILE_SCOPE("Shader_load");

        std::ifstream shaderFile(m_Metadata.Path);
        if (!shaderFile.is_open())
        {
            Core::Logger::LogError("Failed to open shader file: " + m_Metadata.Path);
            return false;
        }

        std::string shaderCode;
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();  // Read entire file into stringstream
        shaderFile.close();
        shaderCode = shaderStream.str();

        // Prepare to split the shader source code into vertex and fragment
        std::string vertexSource;
        std::string fragmentSource;

        std::string delimiterVertex = "#shader vertex";
        std::string delimiterFragment = "#shader fragment";

        size_t vertexPos = shaderCode.find(delimiterVertex);
        size_t fragmentPos = shaderCode.find(delimiterFragment);

        // Check that both directives exist in the shader file
        if (vertexPos == std::string::npos || fragmentPos == std::string::npos)
        {
            LOG_ERROR("Shader file missing #shader vertex or #shader fragment directive: " + m_Metadata.Path +
                   "\n Make sure to split your glsl shader with #shader vertex/fragment.");
            return false;
        }

        // Ensure that vertex shader comes before fragment shader, swap if necessary
        if (fragmentPos < vertexPos)
        {
            std::swap(vertexPos, fragmentPos);
            std::swap(delimiterVertex, delimiterFragment);
        }

        // Extract vertex shader source
        vertexPos += delimiterVertex.size(); // Move past #shader vertex
        fragmentPos += delimiterFragment.size(); // Move past #shader fragment

        vertexSource = shaderCode.substr(vertexPos, fragmentPos - vertexPos - delimiterFragment.size());

        // Extract fragment shader source
        fragmentSource = shaderCode.substr(fragmentPos);

        return LoadFromMemory(vertexSource.c_str(), fragmentSource.c_str());
    }


    bool Shader::LoadFromMemory(const char *vertexSource, const char *fragmentSource)
    {
        // Compile and link shaders
        GLuint vertexShaderID, fragmentShaderID;
        int success;
        char infoLog[512];

        // Vertex shader
        vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderID, 1, &vertexSource, NULL);
        glCompileShader(vertexShaderID);

        // Check errors
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
            Core::Logger::LogError(infoLog, __FILE__, __LINE__);
        }

        // Fragment shader
        fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderID, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShaderID);

        // Check errors
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
            Core::Logger::LogError(infoLog, __FILE__, __LINE__);
        }

        // Link program
        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertexShaderID);
        glAttachShader(m_RendererID, fragmentShaderID);
        glLinkProgram(m_RendererID);

        // Check errors
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
            Core::Logger::LogError(infoLog, __FILE__, __LINE__);
        }

        // Delete shaders as they are no longer necessary because they are linked in the program
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        LOG_INFO("Shader " + m_Metadata.Name + " loaded.");

        return m_Loaded = true;
    }

    void Shader::Use() const
    {
	    glUseProgram(m_RendererID);
    }

    void Shader::End() const
    {
        glUseProgram(0);
    }

    void Shader::SetBool(const std::string& uniformName, bool value)
    {
        glUniform1i(glGetUniformLocation(m_RendererID, uniformName.c_str()), (int)value);
    }

    void Shader::SetFloat(const std::string& uniformName, float value)
    {
        glUniform1f(glGetUniformLocation(m_RendererID, uniformName.c_str()), value);
    }

    void Shader::SetInt(const std::string& uniformName, int value)
    {
        glUniform1i(glGetUniformLocation(m_RendererID, uniformName.c_str()), value);
    }

    void Shader::SetVector2(const std::string& uniformName, glm::vec2 value)
    {
        glUniform2f(glGetUniformLocation(m_RendererID, uniformName.c_str()), value.x, value.y);
    }

    void Shader::SetVector3(const std::string &uniformName, glm::vec3 value)
    {
        glUniform3f(glGetUniformLocation(m_RendererID, uniformName.c_str()), value.x, value.y, value.z);
    }

    void Shader::SetVector4(const std::string& uniformName, glm::vec4 value)
    {
        glUniform4f(glGetUniformLocation(m_RendererID, uniformName.c_str()), value.x, value.y, value.z, value.w);
    }

    void Shader::SetMatrix(const std::string &uniformName, glm::mat4 value)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_RendererID, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    bool Shader::UnLoad()
    {
        glDeleteProgram(m_RendererID);
        return true;
    }

}