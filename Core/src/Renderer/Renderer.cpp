#include "Renderer.h"
#include "Shader.h"
#include "Events/WindowEvents.h"
#include <iostream>

#include "Core/Application.h"

namespace Core::Gfx
{
    Camera Renderer::s_ActiveCamera;
    Light Renderer::s_SceneLight;
    Ref<Shader> Renderer::s_FlatShader;

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

        s_FlatShader = Shader::Create("assets/shaders/Renderer@Flat.glsl");
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
        s_ActiveCamera = cam;
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::DrawModel(Ref<Model> model, glm::mat4 transform)
    {
        auto material = model->GetMaterial();
        if (!material) return;

        if (!material->Shader) material->Shader = s_FlatShader;
        material->Shader->Use();

        material->Shader->SetMatrix("uView", s_ActiveCamera.GetViewMatrix());
        material->Shader->SetMatrix("uProjection", s_ActiveCamera.GetProjectionMatrix());
        material->Shader->SetMatrix("uTransform", transform);

        material->Shader->SetVector3("uMaterialColor", material->Color);

        material->DiffuseTexture->Bind();
        //glBindTexture(GL_TEXTURE_2D, material->DiffuseTexture->GetID());

        for (auto& mesh : model->GetMeshes())
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
            if (!modelComponent.ModelHandle) continue;

            auto& model = Application::Get()->GetCurrentProject().GetRegistry().Get<Model>(modelComponent.ModelHandle);

            DrawModel(model, transformComponent);
        }
    }

}