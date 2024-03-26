#include <GL/glew.h>

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
	void SetVector2(const std::string& uniformName, float value1, float value2);
};