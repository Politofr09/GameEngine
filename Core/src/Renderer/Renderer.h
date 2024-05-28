#include <GL/glew.h>
#include "Shader.h"
#include "Core/Utils.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Camera.h"

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
        Renderer() = default;

        // Initialize OpenGL
        static void Init();

        // Clear the screen
        static void Clear();

        // Begin rendering. Note: this calls Clear()
        static void Begin(const Camera& cam);
        static void End();
        
        // TODO: Make models have materials and materials have reference to shader programs. E.g. model.GetMaterials()[0].GetShader().Use();
        static void DrawModel(Model& model, Shader& shader, glm::mat4 transform);
        static void DrawMesh(Mesh& mesh, Shader& shader, glm::mat4 transform);
    
    private:
        static Camera m_ActiveCamera;
    };

}