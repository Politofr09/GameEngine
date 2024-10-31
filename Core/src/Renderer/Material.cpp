#include "Material.h"
#include "Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Core/Application.h"

#include <filesystem>

namespace Core::Gfx
{
	// TODO: Expand this as we need more uniforms!
	// TODO: Make sure to check the correct ShaderType when we add more and more Shading!
    AssetHandle Material::Create(const std::string& path, const std::string& name)
    {
        std::cout << "Creating material" << std::endl;

        AssetMetadata metadata
        {
            name,
            path,
            UUID()
        };

        Material* material = new Material(metadata, 0, 0, 0);
        
        if (material->Load())
        {
            return Application::Get()->GetCurrentProject().GetRegistry().Track(material);
        }

        return 0;
    }

    bool Material::Load()
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(m_Metadata.Path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
            return false;
        }

        m_AssimpSrcMaterial = scene->mMaterials[scene->mMeshes[0]->mMaterialIndex]; // TODO: support multiple materials per model...

        aiColor3D color;
        if (m_AssimpSrcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        {
            Color = glm::vec3(color.r, color.g, color.b);
        }

        if (m_AssimpSrcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
        {
            Ambient = glm::vec3(color.r, color.g, color.b);
        }

        if (m_AssimpSrcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
        {
            Specular = glm::vec3(color.r, color.g, color.b);
        }

        float shininess = 0.0f;
        if (m_AssimpSrcMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
        {
            Shininess = shininess;

            // Assign Phong shading if shininess is set
            if (shininess > 0.0f)
            {
                m_ShaderType = ShaderType::PhongShading;
            }
            else
            {
                m_ShaderType = ShaderType::FlatShading;
            }
        }
        else
        {
            // Default to FlatShading if no shininess is found
            m_ShaderType = ShaderType::FlatShading;
        }
        Shininess = 1.0f;

        // Create textures that might not be registered
        // To check if a texture already exists we don't have uuid (the material is beeing imported, it doesn't exist in the registry)
        // so we could just check if there is a texture with the same path
        aiString str;
        std::string modelDirectory = std::filesystem::path(GetPath()).parent_path().string() + "/";

        // Attempt to retrieve existing diffuse from registry
        if (m_AssimpSrcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS)
        {
            AssetHandle existingDiffuse = Application::Get()->GetCurrentProject().GetRegistry().FindByPath<Texture>(std::string(modelDirectory + str.C_Str()));
            if (existingDiffuse != 0)
            {
                DiffuseTextureHandle = existingDiffuse;
            }
            else
            {
                DiffuseTextureHandle = Texture::Create(
                    modelDirectory + str.C_Str(),
                    "Texture",
                    "texture_diffuse"
                );
            }
        }

        if (m_AssimpSrcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &str) == AI_SUCCESS)
        {
            AssetHandle existingSpecular = Application::Get()->GetCurrentProject().GetRegistry().FindByPath<Texture>(std::string(modelDirectory + str.C_Str()));
            if (existingSpecular != 0)
            {
                SpecularTextureHandle = existingSpecular;
            }
            else
            {
                SpecularTextureHandle = Texture::Create(
                    modelDirectory + str.C_Str(),
                    "Texture",
                    "texture_specular"
                );
            }
        }

        // Attempt to retrieve existing normals from registry
        if (m_AssimpSrcMaterial->GetTexture(aiTextureType_NORMALS, 0, &str) == AI_SUCCESS)
        {
            AssetHandle existingNormal = Application::Get()->GetCurrentProject().GetRegistry().FindByPath<Texture>(std::string(modelDirectory + str.C_Str()));
            if (existingNormal != 0)
            {
                NormalTextureHandle = existingNormal;
            }
            else
            {
                NormalTextureHandle = Texture::Create(
                    modelDirectory + str.C_Str(),
                    "Texture",
                    "texture_normal"
                );
            }
        }

        m_Loaded = true;

        return true;
    }

    Material::Material(const AssetMetadata& metadata, AssetHandle diffuseTexture, AssetHandle specularTexture, AssetHandle normalTexture)
    : Asset(metadata), DiffuseTextureHandle(diffuseTexture), SpecularTextureHandle(specularTexture), NormalTextureHandle(normalTexture) {}

}