#include <GL/glew.h>
#include <iostream>
#include "Core/Window.h"
#include "Core/Shader.h"
#include "Core/Utils.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Core;

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall()
{
    if (GLuint error = glGetError())
    {
        std::cout << "[OpenGL error]: (" << error << ")" << std::endl;
        return false;
    }
    return true;
}

#define GLCall(x) GLClearError(); \
    x;\
    ASSERT(GLLogCall());

int main()                                                                      
{
    Window* window = new Window(1080, 720, "Hello, abstraction!");              

    float vertices[] = 
    {
        // positions               // colors
        -0.5f, 0.0f,-0.5f,        1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
         0.5f, 0.0f,-0.5f,        0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
         0.5f, 0.0f, 0.5f,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f, 0.0f, 0.5f,        0.2f, 0.5f, 0.3f,    0.0f, 1.0f,
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    Shader shader("res/VertexShader.vs", "res/FragmentShader.fs");
    shader.Use();

    /**** Load texture ****/
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res/wall.jpg", &width, &height, &nrChannels, 0);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    /**** Transformation matrix ****/
    glm::mat4 trans = glm::mat4(1.0f);
    shader.SetMatrix("uTransform", trans);

    /**** View & Projection matrices ****/
    glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f,  1.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    shader.SetMatrix("uView", view);
    float aspectRatio = window->GetWidth() / window->GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspectRatio, 1.0f, 1000.0f);
    shader.SetMatrix("uProjection", projection);

    /**** Main loop ****/
    while (!window->ShouldClose())             
    {
        const float cameraSpeed = 0.05f;

        // Update camera position based on user input
        if (glfwGetKey(window->GetHandle(), GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos.z -= cameraSpeed;
            cameraTarget.z -= cameraSpeed;
        }
        if (glfwGetKey(window->GetHandle(), GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos.z += cameraSpeed;
            cameraTarget.z += cameraSpeed;
        }
        if (glfwGetKey(window->GetHandle(), GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos.x -= cameraSpeed;
            cameraTarget.x -= cameraSpeed;
        }
        if (glfwGetKey(window->GetHandle(), GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos.x += cameraSpeed;
            cameraTarget.x += cameraSpeed;
        }

        // Calculate view matrix
        view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        shader.SetMatrix("uView", view);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);          
        /* Render here */
        
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        window->Update();
    }

    window->Close();

    return 0;
}