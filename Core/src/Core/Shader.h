#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	GLuint ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void Use();
	void End();

	// Uniform loading functions
	void SetBool(const std::string& uniformName, bool value);
	void SetFloat(const std::string& uniformName, float value);
	void SetInt(const std::string& uniformName, int value);
	void SetVector2(const std::string& uniformName, glm::vec2 value);
	void SetVector3(const std::string& uniformName, glm::vec3 value);
	void SetMatrix(const std::string& uniformName, glm::mat4 value);
};