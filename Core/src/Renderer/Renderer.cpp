#include "Renderer.h"
#include "Shader.h"
#include "Events/WindowEvents.h"
#include <iostream>

#include "Core/Application.h"

namespace Core::Gfx
{
    Camera Renderer::m_ActiveCamera;
    Light Renderer::m_SceneLight;
    FlatShading Renderer::m_FlatShading;
    PhongShading Renderer::m_PhongShading;

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
    
    Renderer::Renderer()
    {
    }

    void Renderer::Init()
    {
        GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

        m_FlatShading.Load();
        //registry.Track(m_FlatShading.ShaderProgram);

        m_PhongShading.Load();
        //registry.Track(m_PhongShading.ShaderProgram);
    }

    void Renderer::EnableCulling()
    {
        glEnable(GL_CULL_FACE);
    }

    void Renderer::DisableCulling()
    {
        glDisable(GL_CULL_FACE);
    }

    void Renderer::EnableDepthTesting()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::DisableDepthTesting()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void Renderer::EnableWireframeMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void Renderer::DisableWireframeMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void Renderer::SetBackgroundColor(glm::vec3 color)
    {
        GLCall(glClearColor(color.x, color.y, color.z, 1.0f));
    }
    
    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::BeginScene(const Camera& cam)
    {
        m_ActiveCamera = cam;
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::DrawModel(Model& model, glm::mat4 transform)
    {
        // Retrieve the material and its shader type
        auto& material = *Application::Get()->GetCurrentProject().GetRegistry().Get<Material>(model.GetMaterialHandle());

        // Set up the shader based on the material's shader type
        switch (material.m_ShaderType)
        {
        case ShaderType::FlatShading:
            m_FlatShading.Apply(material);
   
            // Set additional uniforms that are common to all shaders
            m_FlatShading.ShaderProgram->SetMatrix("uView", m_ActiveCamera.GetViewMatrix());
            m_FlatShading.ShaderProgram->SetMatrix("uProjection", m_ActiveCamera.GetProjectionMatrix());
            m_FlatShading.ShaderProgram->SetMatrix("uTransform", transform);
            break;

        case ShaderType::PhongShading:
            m_PhongShading.Apply(material);

            // Set additional uniforms that are common to all shaders
            m_PhongShading.ShaderProgram->SetMatrix("uView", m_ActiveCamera.GetViewMatrix());
            m_PhongShading.ShaderProgram->SetMatrix("uProjection", m_ActiveCamera.GetProjectionMatrix());
            m_PhongShading.ShaderProgram->SetMatrix("uTransform", transform);
            m_PhongShading.ShaderProgram->SetVector3("ligth_position", m_SceneLight.Position);
            m_PhongShading.ShaderProgram->SetVector3("light_color", m_SceneLight.Color);
            m_PhongShading.ShaderProgram->SetVector3("view_position", m_ActiveCamera.GetPosition());
            break;

        case ShaderType::None:
            m_FlatShading.Apply(material);
            glBindTexture(GL_TEXTURE_2D, 0);
            m_FlatShading.ShaderProgram->SetVector3("material_color", glm::vec3(1.0f));

            // Set additional uniforms that are common to all shaders
            m_FlatShading.ShaderProgram->SetMatrix("uView", m_ActiveCamera.GetViewMatrix());
            m_FlatShading.ShaderProgram->SetMatrix("uProjection", m_ActiveCamera.GetProjectionMatrix());
            m_FlatShading.ShaderProgram->SetMatrix("uTransform", transform);
            break;
            // Add other shader types as needed
        }

        for (auto& mesh : model.GetMeshes())
        {
            mesh.GetVertexArray().Bind();

            GLCall(glDrawElements(GL_TRIANGLES, mesh.GetVertexArray().GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
        }
    }

    void Renderer::RenderScene(Scene& scene)
    {
        using namespace Ecs;
        ECS registry = scene.GetRegistry();

        // Todo: batch all models by their shading type
        auto view = registry.GetView<TransformComponent, ModelComponent>();
        for (auto e : view)
        {
            auto& transformComponent = registry.GetComponent<TransformComponent>(e);
            auto& modelComponent = registry.GetComponent<ModelComponent>(e);
            if (modelComponent.ModelHandle == 0) continue;

            auto& model = *Application::Get()->GetCurrentProject().GetRegistry().Get<Model>(modelComponent.ModelHandle);

            DrawModel(model, transformComponent); // Use of mat4 operator (nice)
        }
    }

}