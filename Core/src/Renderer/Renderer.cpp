#include "Renderer.h"
#include "Shader.h"
#include "Events/WindowEvents.h"
#include <iostream>

namespace Core::Gfx
{

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

    Camera Renderer::m_ActiveCamera{};
    
    Renderer::Renderer()
    {
    }

    void Renderer::Init()
    {
        glEnable(GL_DEPTH_TEST);
        GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    }
    
    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Begin(const Camera& cam)
    {
        m_ActiveCamera = cam;
    }

    //void Renderer::BeginFBO(const Camera& cam, const FrameBuffer& fbo)
    //{
    //    fbo.Bind();
    //    m_ActiveCamera = cam;

    //    // Extract existing FOV from m_ActiveCamera's projection matrix
    //    float existingFOV = glm::degrees(2.0f * atan(1.0f / m_ActiveCamera.GetProjectionMatrix()[1][1]));

    //    float aspectRatio = (float)fbo.GetWidth() / fbo.GetHeight();
    //    m_ActiveCamera.SetProjectionMatrix(glm::perspective(glm::radians(existingFOV), aspectRatio, 0.1f, 1000.0f));

    //    glViewport(0, 0, fbo.GetWidth(), fbo.GetHeight());
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the framebuffer
    //}

    void Renderer::End()
    {
    }

    void Renderer::DrawModel(Model& model, glm::mat4 transform)
    {
        Shader shader = model.GetMaterial().GetShader();
        shader.Use();
        Texture texture = model.GetMaterial().GetTexture();
        texture.Bind();
        glActiveTexture(GL_TEXTURE0);

        shader.SetMatrix("uView", m_ActiveCamera.GetViewMatrix());
        shader.SetMatrix("uProjection", m_ActiveCamera.GetProjectionMatrix());
        shader.SetMatrix("uTransform", transform);
        
        for (auto& mesh : model.GetMeshes())
        {
            mesh.GetVertexArray().Bind();
            mesh.GetIndexBuffer().Bind();

            GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
        }
    }

    void Renderer::OnViewportResize(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

}