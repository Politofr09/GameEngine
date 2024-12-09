#include "Material.h"
#include "Base.h"
#include "Core/Application.h"

#include <filesystem>

#include "Model.h"

namespace Core::Gfx
{   

    Ref<Material> Material::Create(const std::string& path)
    {
        Ref<Material> material = CreateRef<Material>();
        material->Path = path;
        material->Deserialize();
        return material;
    }

    void Material::Deserialize()
    {
        std::ifstream fin(Path);
        if (!fin.is_open())
        {
            LOG_ERROR("Couldn't load " + Path + " material file");
        }
        fin.close();

        YAML::Node root = YAML::Load(fin);

        YAML::Node materialNode = root["Material"];
        ID = materialNode.as<uint64_t>();
        
        Name = materialNode["Name"].as<std::string>();
        Color = YAMLToVec3(materialNode["Color"]);
        Ambient = YAMLToVec3(materialNode["Ambient"]);
        Diffuse = YAMLToVec3(materialNode["Diffuse"]);
        Specular = YAMLToVec3(materialNode["Specular"]);
        Shininess = materialNode["Shininess"].as<float>();

        // Texture assignment
        std::string diffusePath = materialNode["DiffuseTexture"].as<std::string>();
        std::string specularPath = materialNode["SpecularTexture"].as<std::string>();
        std::string normalPath = materialNode["NormalTexture"].as<std::string>();
    
        auto& registry = Application::Get()->GetCurrentProject().GetRegistry();
        
        Ref<Texture> existingDiffuse = registry.Get<Texture>(diffusePath);
        if (existingDiffuse->GetMetadata().ID)
        {
            DiffuseTexture = existingDiffuse;
        }
        else
        { 
            DiffuseTexture = Texture::Create(diffusePath);
            registry.Track<Texture>(DiffuseTexture);
        }
        
        Ref<Texture> existingSpecular = registry.Get<Texture>(specularPath);
        if (existingSpecular->GetMetadata().ID)
        {
            SpecularTexture = existingSpecular;
        }
        else
        {
            SpecularTexture = Texture::Create(diffusePath);
            registry.Track<Texture>(SpecularTexture);
        }

        Ref<Texture> existingNormal = registry.Get<Texture>(normalPath);
        if (existingNormal->GetMetadata().ID)
        {
            NormalTexture = existingNormal;
        }
        else
        {
            NormalTexture = Texture::Create(diffusePath);
            registry.Track<Texture>(NormalTexture);
        }
    }

    void Material::Serialize() const
    {
        YAML::Emitter out;  
        out << YAML::BeginMap;
        {
            out << YAML::Key << "Material" << YAML::Value << ID;
            out << YAML::Key << "Name" << YAML::Value << Name;
            
            out << YAML::Key << "Color" << YAML::Value << Color;
            out << YAML::Key << "Ambient" << YAML::Value << Ambient;
            out << YAML::Key << "Diffuse" << YAML::Value << Diffuse;
            out << YAML::Key << "Specular" << YAML::Value << Specular;
            out << YAML::Key << "Shininess" << YAML::Value << Shininess;

            out << YAML::Key << "DiffuseTexture"  << DiffuseTexture->GetMetadata().ID;
            out << YAML::Key << "SpecularTexture" << SpecularTexture->GetMetadata().ID;
            out << YAML::Key << "NormalTexture"   << NormalTexture->GetMetadata().ID;
        }
        out << YAML::EndMap;

        // Write to the file
        std::ofstream fout(Path);
        fout << out.c_str();
        fout.close();
    }

    Ref<Material> Material::CreateFromMemory(const aiScene* scene, aiMaterial* mat, const std::string& modelFileName)
    {
        Ref<Material> material = CreateRef<Material>();
        material->LoadFromMemory(scene, mat, modelFileName);
        return material;
    }
    
    bool Material::LoadFromMemory(const aiScene* scene, aiMaterial* material, const std::string& modelFileName)
    {
        Path = modelFileName;

        aiColor3D color;
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        {
            Color = glm::vec3(color.r, color.g, color.b);
        }

        if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
        {
            Ambient = glm::vec3(color.r, color.g, color.b);
        }

        if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
        {
            Specular = glm::vec3(color.r, color.g, color.b);
        }

        float shininess = 0.0f;
        if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
        {
            Shininess = shininess;
        }

        RetrieveTextures(scene, material);

        return true;
    }

    void Material::RetrieveTextures(const aiScene* scene, const aiMaterial* material)
    {

        bool hasEmbedded = scene->mTextures != nullptr;

        // Create textures that might not be registered
        aiString str;
        std::string relativeDirectory = std::filesystem::path(Path).parent_path().string() + "/";

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS && !hasEmbedded)
        {
            std::filesystem::path texturePath = str.C_Str();
            // Check if it's a relative path
            // if so, add the relative directory
            if (std::filesystem::canonical(texturePath) != texturePath)
            {
                texturePath = std::string(relativeDirectory + texturePath.string());
            }

            Ref<Texture> existingDiffuse = Application::Get()->GetCurrentProject().GetRegistry().Get<Texture>(texturePath.string());
            if (existingDiffuse->GetMetadata().ID)
            {
                DiffuseTexture = existingDiffuse;
            }
            else
            {
                DiffuseTexture = Texture::Create(relativeDirectory + str.C_Str());
                Application::Get()->GetCurrentProject().GetRegistry().Track(DiffuseTexture);
            }
        }


        if (material->GetTexture(aiTextureType_SPECULAR, 0, &str) == AI_SUCCESS)
        {
            std::filesystem::path texturePath = str.C_Str();
            // Check if it's a relative path
            // if so, add the relative directory
            if (std::filesystem::canonical(texturePath) != texturePath)
            {
                texturePath = std::string(relativeDirectory + texturePath.string());
            }

            Ref<Texture> existingSpecular = Application::Get()->GetCurrentProject().GetRegistry().Get<Texture>(texturePath.string());
            if (existingSpecular->GetMetadata().ID)
            {
                SpecularTexture = existingSpecular;
            }
            else
            {
                SpecularTexture = Texture::Create(relativeDirectory + str.C_Str());
                Application::Get()->GetCurrentProject().GetRegistry().Track(DiffuseTexture);
            }
        }

        if (material->GetTexture(aiTextureType_NORMALS, 0, &str) == AI_SUCCESS)
        {
            std::filesystem::path texturePath = str.C_Str();
            // Check if it's a relative path
            // if so, add the relative directory
            if (std::filesystem::canonical(texturePath) != texturePath)
            {
                texturePath = std::string(relativeDirectory + texturePath.string());
            }

            Ref<Texture> existingNormal = Application::Get()->GetCurrentProject().GetRegistry().Get<Texture>(texturePath.string());
            if (existingNormal->GetMetadata().ID)
            {
                NormalTexture = existingNormal;
            }
            else
            {
                NormalTexture = Texture::Create(relativeDirectory + str.C_Str());
            }
        }
    }

}