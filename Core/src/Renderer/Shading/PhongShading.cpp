#include "PhongShading.h"
#include "Core/Application.h"

namespace Core::Gfx
{

    static const char* vertexSource = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoord;

        out vec3 vNormal;
        out vec3 vPos;
        out vec2 uv;

        uniform mat4 uTransform;
        uniform mat4 uView;
        uniform mat4 uProjection;

        void main()
        {
            vNormal = mat3(transpose(inverse(uTransform))) * aNormal;  // Adjust normal based on the model's transformation
            vPos = vec3(uTransform * vec4(aPos, 1.0));  // Transformed vertex position
            uv = aTexCoord;
            gl_Position = uProjection * uView * uTransform * vec4(aPos, 1.0);
        }
    )";

    static const char* fragmentSource = R"(
        #version 330 core

        out vec4 FragColor;
        in vec3 vNormal;
        in vec3 vPos;
        in vec2 uv;

        uniform sampler2D material_diffuse_texture;
        uniform vec3 material_color;

        // Phong model uniforms
        uniform vec3 light_position;
        uniform vec3 light_color;
        uniform vec3 view_position;

        uniform vec3 material_ambient;
        uniform vec3 material_diffuse;
        uniform vec3 material_specular;
        uniform float material_shininess;

        void main()
        {
            // Ambient
            vec3 ambient = material_ambient * light_color;

            // Diffuse
            vec3 norm = normalize(vNormal);
            vec3 lightDir = normalize(light_position - vPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * material_diffuse * light_color;

            // Specular
            vec3 viewDir = normalize(view_position - vPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 100/material_shininess);
            vec3 specular = material_specular * spec * light_color;

            // Combine results
            vec3 lighting = ambient + diffuse + specular;
            vec4 textureColor = texture(material_diffuse_texture, uv);
            FragColor = vec4(lighting, 1.0) * textureColor * vec4(material_color, 1.0);
        }
    )";


	bool PhongShading::Load()
	{
        ShaderProgram = new Shader();
        ShaderProgram->SetName("PhongShader");
        return ShaderProgram->LoadFromMemory(vertexSource, fragmentSource);
	}

	void PhongShading::Apply(const Material& material)
	{
        ShaderProgram->Use();

        // Set material properties
        ShaderProgram->SetVector3("material_ambient", material.Ambient);
        ShaderProgram->SetVector3("material_diffuse", material.Diffuse);
        ShaderProgram->SetVector3("material_specular", material.Specular);
        ShaderProgram->SetFloat("material_shininess", material.Shininess);
        ShaderProgram->SetVector3("material_color", material.Color);

        Texture DiffuseTexture = *Application::Get()->GetCurrentProject().GetRegistry().Get<Texture>(material.DiffuseTextureHandle);

        // Bind diffuse texture if loaded
        DiffuseTexture.Bind();
        glActiveTexture(GL_TEXTURE0);
        ShaderProgram->SetInt("material_diffuse_texture", 0);
	}

}