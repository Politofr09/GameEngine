#include "Renderer.h"
#include "Shader.h"
#include "Events/WindowEvents.h"
#include <iostream>

#include "Core/Application.h"

namespace Core::Gfx
{
    Camera Renderer::s_ActiveCamera;
    Light Renderer::s_SceneLight;
    Ref<Shader> Renderer::s_PhongShader;
    Ref<Shader> Renderer::s_GridShader;
    Renderer::GridSettings Renderer::s_GridSettings;

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

        s_PhongShader = Shader::Create("assets/shaders/Renderer@Phong.glsl");

        s_GridShader = Shader::Create("assets/shaders/Renderer@Grid.glsl");
 
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

    void Renderer::DrawGrid()
    {
        s_GridShader->Use();

        s_GridShader->SetFloat("uGridSize", s_GridSettings.gridSize);
        s_GridShader->SetFloat("uGridCellSize", s_GridSettings.gridCellSize);
        s_GridShader->SetVector4("uGridColorThin", s_GridSettings.gridColorThin);
        s_GridShader->SetVector4("uGridColorThick", s_GridSettings.gridColorThick);
        s_GridShader->SetFloat("uGridMinPixelsBetweenCells", s_GridSettings.gridMinPixelsBetweenCells);

        s_GridShader->SetMatrix("uView", s_ActiveCamera.GetViewMatrix());
        s_GridShader->SetMatrix("uProjection", s_ActiveCamera.GetProjectionMatrix());

        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 6); // Draw two triangles (6 vertices)
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
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

        if (!material->Shader) material->Shader = s_PhongShader;
        material->Shader->Use();

        material->Shader->SetMatrix("uView", s_ActiveCamera.GetViewMatrix());
        material->Shader->SetMatrix("uProjection", s_ActiveCamera.GetProjectionMatrix());
        material->Shader->SetMatrix("uTransform", transform);

        material->Shader->SetVector3("uMaterialColor", material->Color);
        material->Shader->SetVector3("uMaterialAmbient", material->Ambient);
        material->Shader->SetVector3("uMaterialDiffuse", material->Diffuse);
        material->Shader->SetVector3("uMaterialSpecular", material->Specular);
        material->Shader->SetFloat("uMaterialShininess", material->Shininess);

        material->Shader->SetVector3("uLightPosition", s_SceneLight.Position);
        material->Shader->SetVector3("uLightColor", s_SceneLight.Color);
        material->Shader->SetVector3("uViewPosition", s_ActiveCamera.GetPosition());

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

        DrawGrid();
    }

}