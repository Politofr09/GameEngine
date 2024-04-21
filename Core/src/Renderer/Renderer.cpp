#include "Renderer.h"
#include <iostream>

void Core::Gfx::GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool Core::Gfx::GLLogCall()
{
    if (GLuint error = glGetError())
    {
        std::cout << "[OpenGL error]: (" << error << ")" << std::endl;
        return false;
    }
    return true;
}

void Core::Gfx::Renderer::Clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Core::Gfx::Renderer::Draw(const VertexArray& va, const IndexBuffer& ib)
{
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}