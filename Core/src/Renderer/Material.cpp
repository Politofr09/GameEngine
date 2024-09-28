#include "Material.h"

namespace Core::Gfx
{
	// TODO: Expand this as we need more uniforms!
	// TODO: Make sure to check the correct ShaderType when we add more and more Shading!
    Material& Material::CreateFromAssimp(AssetRegistry& registry, aiMaterial* mat, const std::string& directory)
    {
        Material* material = new Material();
        aiString str;

        // Load diffuse texture
        if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS)
        {
            material->DiffuseTexture = Texture::Create(registry, directory + "/" + str.C_Str(),
                "model_texture_diffuse_" + std::string(mat->GetName().C_Str()),
                "texture_diffuse");
        }

        // Load specular texture
        if (mat->GetTexture(aiTextureType_SPECULAR, 0, &str) == AI_SUCCESS)
        {
            material->SpecularTexture = Texture::Create(registry, directory + "/" + str.C_Str(),
                "model_texture_specular_" + std::string(mat->GetName().C_Str()),
                "texture_specular");
        }

        // Load normal map texture
        if (mat->GetTexture(aiTextureType_NORMALS, 0, &str) == AI_SUCCESS)
        {
            material->NormalTexture = Texture::Create(registry, directory + "/" + str.C_Str(),
                "model_texture_normal_" + std::string(mat->GetName().C_Str()),
                "texture_normal");
        }

        // Load color and material properties
        aiColor3D color;
        if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        {
            material->Color = glm::vec3(color.r, color.g, color.b);
        }

        if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
        {
            material->Ambient = glm::vec3(color.r, color.g, color.b);
        }

        if (mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
        {
            material->Specular = glm::vec3(color.r, color.g, color.b);
        }

        float shininess = 0.0f;
        if (mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
        {
            material->Shininess = shininess;

            // Assign Phong shading if shininess is set
            if (shininess > 0.0f)
            {
                material->m_ShaderType = ShaderType::PhongShading;
            }
            else
            {
                material->m_ShaderType = ShaderType::FlatShading;
            }
        }
        else
        {
            // Default to FlatShading if no shininess is found
            material->m_ShaderType = ShaderType::FlatShading;
        }
        material->Shininess = 1.0f;

        std::cout << "Material.Color: " << material->Color.x << " " << material->Color.y << " " << material->Color.z << std::endl;
        std::cout << "Material.Ambient: " << material->Ambient.x << " " << material->Ambient.y << " " << material->Ambient.z << std::endl;
        std::cout << "Material.Diffuse: " << material->Diffuse.x << " " << material->Diffuse.y << " " << material->Diffuse.z << std::endl;
        std::cout << "Material.Specular: " << material->Specular.x << " " << material->Specular.y << " " << material->Specular.z << std::endl;
        std::cout << "Material.Shininess:" << material->Shininess << std::endl;

        registry.Track(material);
        return *material;
    }

}
