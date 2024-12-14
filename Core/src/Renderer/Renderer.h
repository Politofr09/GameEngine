#include <GL/glew.h>
#include "Shader.h"
#include "Core/Utils.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Light.h"
#include "Core/Scene.h"

#pragma once

#define GLCall(x) ::Core::Gfx::GLClearError(); \
    x;\
    ASSERT(::Core::Gfx::GLLogCall());

namespace Core::Gfx
{
    void GLClearError();
    bool GLLogCall();

    class Renderer
    {
    public:
        Renderer();
        static void Init();

        static void SetBackgroundColor(glm::vec3 color);
        static void Clear();

        static void BeginScene(const Camera& cam);
        static void EndScene();

        static void SetLight(const Light& light) { s_SceneLight = light; }

        static void DrawModel(Ref<Model> model, glm::mat4 transform);

        static void RenderScene(Scene& scene);

        //static void Submit(Shader& shader, VertexArray& va, glm::mat4& transform);

        // Settings
        static void EnableCulling();
        static void DisableCulling();

        static void EnableDepthTesting();
        static void DisableDepthTesting();

        static void EnableWireframeMode();
        static void DisableWireframeMode();

        static void DrawGrid();
        
        struct GridSettings
        {
            float gridSize = 100.0f;               // uGridSize
            float gridCellSize = 0.8f;            // uGridCellSize
            glm::vec4 gridColorThin = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);  // uGridColorThin
            glm::vec4 gridColorThick = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // uGridColorThick
            float gridMinPixelsBetweenCells = 2.0f; // uGridMinPixelsBetweenCells
        };

        static GridSettings s_GridSettings;

    private:
        static Camera s_ActiveCamera;
        static Light s_SceneLight;
        static Ref<Shader> s_PhongShader;
        static Ref<Shader> s_GridShader;

    };

}