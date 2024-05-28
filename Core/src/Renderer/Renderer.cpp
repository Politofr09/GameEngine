#include "Renderer.h"
#include "Shader.h"
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

    Camera Renderer::m_ActiveCamera{ 0 };

    void Renderer::Init()
    {
        glEnable(GL_DEPTH_TEST); // <-- it works here; anywhere above it doesn't work
        GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        //glClearDepth(1.0f);
    }

    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Begin(const Camera& cam)
    {
        m_ActiveCamera = cam;
    }

    void Renderer::End()
    {
    }

    void Renderer::DrawModel(Model& model, Shader& shader, glm::mat4 transform)
    {
        for (auto& mesh : model.GetMeshes())
        {
            DrawMesh(mesh, shader, transform);
        }
    }

    void Renderer::DrawMesh(Mesh& mesh, Shader& shader, glm::mat4 transform)
    {
        //// Turn on wireframe mode
        //glPolygonMode(GL_FRONT, GL_LINE);
        //glPolygonMode(GL_BACK, GL_LINE);

        shader.Use();

        // Upload camera matrices
        shader.SetMatrix("uView", m_ActiveCamera.GetViewMatrix());
        shader.SetMatrix("uProjection", m_ActiveCamera.GetProjectionMatrix());

        mesh.GetVertexArray().Bind();
        mesh.GetIndexBuffer().Bind();

        unsigned int diffuseNr = 0, specularNr = 0, ambientNr = 0, emissionNr = 0, normalNr = 0, heightNr = 0, noiseNr = 0;

        for (unsigned int i = 0; i < mesh.textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            TextureType type = mesh.textures[i].GetType();
            std::string type_str = ""; // Shaders will use 'uniform sampler2D texture_diffuse1;'

            unsigned int textureUnit = 0;
            switch (type)
            {
            case TEXTURE_DIFFUSE:
                textureUnit = diffuseNr++;
                type_str = "texture_diffuse";
                break;
            case TEXTURE_SPECULAR:
                textureUnit = specularNr++;
                type_str = "texture_specular";
                break;
            case TEXTURE_AMBIENT:
                textureUnit = ambientNr++;
                type_str = "texture_ambient";
                break;
            case TEXTURE_EMISSION:
                textureUnit = emissionNr++;
                type_str = "texture_emission";
                break;
            case TEXTURE_NORMAL:
                textureUnit = normalNr++;
                type_str = "texture_normal";
                break;
            case TEXTURE_HEIGHT:
                textureUnit = heightNr++;
                type_str = "texture_height";
                break;
            case TEXTURE_NOISE:
                textureUnit = noiseNr++;
                type_str = "texture_noise";
                break;
            default:
                break;
            }

            shader.SetInt(type_str + std::to_string(textureUnit), i);
            mesh.textures[i].Bind();
        }

        // Load the uniform into the shader
        shader.SetMatrix("uTransform", transform);

        glActiveTexture(GL_TEXTURE0);
        GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));

        //// Turn off wireframe mode
        //glPolygonMode(GL_FRONT, GL_FILL);
        //glPolygonMode(GL_BACK, GL_FILL);
    }

}