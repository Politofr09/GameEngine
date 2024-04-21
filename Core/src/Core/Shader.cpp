#include "Shader.h"
#include "Utils.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    Load(vertexPath, fragmentPath);
}

void Shader::Load(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

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
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // Compile and link shaders
    GLuint vertexShaderID, fragmentShaderID;
    int success;
    char infoLog[512];

    // Vertex shader
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vShaderCode, NULL);
    glCompileShader(vertexShaderID);

    // Check errors
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        Core::Logger::LogError(infoLog, __FILE__, __LINE__);
    }

    // Vertex shader
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShaderID);

    // Check errors
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
        Core::Logger::LogError(infoLog, __FILE__, __LINE__);
    }

    // Link program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShaderID);
    glAttachShader(ID, fragmentShaderID);
    glLinkProgram(ID);

    // Check errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        Core::Logger::LogError(infoLog, __FILE__, __LINE__);
    }

    // Delete shaders as they are no longer necessary because they are linked in the program
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void Shader::Use()
{
	glUseProgram(ID);
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
    glUniform1f(glGetUniformLocation(ID, uniformName.c_str()), value);
}

void Shader::SetInt(const std::string& uniformName, int value)
{
    glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), value);
}

void Shader::SetVector2(const std::string& uniformName, glm::vec2 value)
{
    glUniform2f(glGetUniformLocation(ID, uniformName.c_str()), value.x, value.y);
}

void Shader::SetVector3(const std::string &uniformName, glm::vec3 value)
{
    glUniform3f(glGetUniformLocation(ID, uniformName.c_str()), value.x, value.y, value.z);
}

void Shader::SetMatrix(const std::string &uniformName, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
