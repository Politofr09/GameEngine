#include <GL/glew.h>
#include "Shader.h"
#include "Core/Utils.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Light.h"

#include "Shading/FlatShading.h"
#include "Shading/PhongShading.h"

#pragma once

#define GLCall(x) Core::Gfx::GLClearError(); \
    x;\
    ASSERT(Core::Gfx::GLLogCall());

namespace Core::Gfx
{
    void GLClearError();
    bool GLLogCall();

    class Renderer
    {
    public:
        Renderer();

        static void Init(AssetRegistry& registry);

        static void SetBackgroundColor(glm::vec3 color);

        static void Clear();

        static void Begin(const Camera& cam);
        static void End();

        static void SetLight(const Light& light) { m_SceneLight = light; }

        // TODO: Make models have materials and materials have reference to shader programs. E.g. model.GetMaterials()[0].GetShader().Use();
        static void DrawModel(Model& model, glm::mat4 transform);
    
        static void OnViewportResize(int width, int height); // Call this when window / framebuffer changes

        // Settings
        static void EnableCulling();
        static void DisableCulling();

        static void EnableDepthTesting();
        static void DisableDepthTesting();

        static void EnableWireframeMode();
        static void DisableWireframeMode();

    private:
        static Camera m_ActiveCamera;
        static Light m_SceneLight;
        static FlatShading m_FlatShading;
        static PhongShading m_PhongShading;
    };

}