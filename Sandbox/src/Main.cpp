#include <GL/glew.h>
#include <iostream>
#include "Core/Window.h"
#include "Core/Shader.h"
#include "Core/Utils.h"

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
        -0.5f, -0.5f, 0.0f,        1.0f, 0.0f, 0.0f,    // bottom right, red
         0.5f, -0.5f, 0.0f,        0.0f, 1.0f, 0.0f,    // bottom left, green
         0.5f,  0.5f, 0.0f,        0.0f, 0.0f, 1.0f,    // top, blue
        -0.5f,  0.5f, 0.0f,        0.2f, 0.5f, 0.3f,
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    Shader shader("res/VertexShader.vs", "res/FragmentShader.fs");
    shader.Use();

    /* Loop until the user closes the window */
    while (!window->ShouldClose())             
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);          
        /* Render here */

        shader.SetFloat("time", glfwGetTime());
        shader.SetVector2("resolution", (float)window->GetWidth(), (float)window->GetHeight());

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        window->Update();
    }

    window->Close();

    return 0;
}