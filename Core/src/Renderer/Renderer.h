#include <GL/glew.h>
#include "Shader.h"
#include "Core/Utils.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Camera.h"
#include "FrameBuffer.h"

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

        // Initialize OpenGL
        static void Init();

        // Clear the screen
        static void Clear();

        // Begin rendering
        static void Begin(const Camera& cam);
        static void End();

        //static void BeginFBO(const Camera& cam, const FrameBuffer& fbo);
        //static void EndFBO(const FrameBuffer& fbo);

        // TODO: Make models have materials and materials have reference to shader programs. E.g. model.GetMaterials()[0].GetShader().Use();
        static void DrawModel(Model& model, glm::mat4 transform);
    
        static void OnEvent(Events::Event* event);

    private:
        static Camera m_ActiveCamera;
    };

}