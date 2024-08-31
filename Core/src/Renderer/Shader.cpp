#include "Shader.h"
#include "Utils.h"

using namespace Core::Gfx;

Shader &Core::Gfx::Shader::Create(const std::string &vertexPath, const std::string &fragmentPath, const std::string &name)
{
    Shader* shader = new Shader(vertexPath, fragmentPath, name);

    if (shader->Load())
    {
        TRACK_RESOURCE(shader);
    }
    
    return *shader;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& name)
{
    m_FileDirectory = vertexPath;
    m_VertexPath = vertexPath;
    m_FragmentPath = fragmentPath;
    
    m_Name = name;
    if (m_Name.empty()) m_Name = GetType() + std::to_string(ID);
}

bool Shader::Load()
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    try
    {
        vShaderFile.open(m_VertexPath);
        fShaderFile.open(m_FragmentPath);

        if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
            throw std::runtime_error("Failed to open shader file.");
        }

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        Core::Logger::LogError(e.what());
        return false;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    return LoadFromMemory(vShaderCode, fShaderCode);
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

    LOG_INFO("Shader " + m_Name + " loaded.");

    return m_Loaded = true;
}

void Shader::Use()
{
	glUseProgram(m_RendererID);
}

void Shader::End()
{
    glUseProgram(0);
}

void Shader::SetBool(const std::string& uniformName, bool value)
{
    glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), (int)value);
    
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

void Core::Gfx::Shader::SetVector4(const std::string& uniformName, glm::vec4 value)
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