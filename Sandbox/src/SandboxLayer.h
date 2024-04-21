#include <GL/glew.h>
#include "Core/Layer.h"
#include "Core/Shader.h"
#include "Core/Camera.h"
#include "Core/Utils.h"
#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma once

#define GLCall(x) GLClearError(); \
    x;\
    ASSERT(GLLogCall());

class SandboxLayer : public Core::Layer
{
public:
    SandboxLayer(Core::Window* window) : Layer(window) {};
	
    void OnAttach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Core::Events::Event& event) override;
	void OnDettach() override;

private:
    float vertices[32] =
    {
        // positions               // colors
        -0.5f, 0.0f,-0.5f,        1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
         0.5f, 0.0f,-0.5f,        0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
         0.5f, 0.0f, 0.5f,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f, 0.0f, 0.5f,        0.2f, 0.5f, 0.3f,    0.0f, 1.0f,
    };

    unsigned int indices[6] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GLuint VAO, VBO, IBO, textureID = 0;

    glm::mat4 trans= glm::mat4();
    Core::Camera cam;

    Shader shader;

    void UpdateCameraController();
    void ShowCameraControlImgui(bool* p_open);
};