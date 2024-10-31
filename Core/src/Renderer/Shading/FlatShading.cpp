#include "FlatShading.h"
#include "Core/Application.h"

namespace Core::Gfx
{

    // TODO: Maybe add some kind of mechanism to load internal engine resources like default shaders
    // It's proabably better to load from a file
    static const char* vertexSource = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoord;

        out vec3 vNormal;
        out vec2 uv;

        uniform mat4 uTransform;
        uniform mat4 uView;
        uniform mat4 uProjection;

        void main()
        {
            vNormal = aNormal;
            uv = aTexCoord;
            gl_Position = uProjection * uView * uTransform * vec4(aPos, 1.0);
        }
    )";

    static const char* fragmentSource = R"(
        #version 330 core

        out vec4 FragColor;
        in vec3 vNormal;
        in vec2 uv;

        uniform sampler2D material_diffuse_texture;
        uniform vec3 material_color;

        void main()
        {
            FragColor = texture(material_diffuse_texture, uv);
        }
    )";

    bool FlatShading::Load()
    {
        ShaderProgram = new Shader();
        ShaderProgram->SetName("FlatShader");
        return ShaderProgram->LoadFromMemory(vertexSource, fragmentSource);
    }

    void FlatShading::Apply(const Material& material)
    {
        ShaderProgram->Use();
        ShaderProgram->SetVector3("material_color", material.Color);

        Texture DiffuseTexture = *Application::Get()->GetCurrentProject().GetRegistry().Get<Texture>(material.DiffuseTextureHandle);
        DiffuseTexture.Bind();
        glEnable(GL_TEXTURE_2D);
    }

}