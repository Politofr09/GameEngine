#include "AssetRegistry.h"
#include "Utils.h"
#include <algorithm>

#include "Renderer/Texture.h"
#include "Renderer/Shader.h"
#include "Renderer/Font.h"
#include "Renderer/Model.h"
#include "Renderer/Material.h"

namespace Core
{

    AssetHandle AssetRegistry::Track(Asset* asset)
    {
        AssetHandle handle = asset->GetID();


        if (!m_Assets[handle])
        {
            m_Assets[handle] = asset;
        }
        else
        {
            LOG_WARNING("There is already an asset with that handle, keeping the other asset");
        }
        
        return handle;
    }

    void AssetRegistry::Kill(Asset* asset)
    {
        auto it = m_Assets.find(asset->GetID());
        if (it != m_Assets.end())
        {
            delete asset;
            m_Assets.erase(it);
        }
    }

    void AssetRegistry::Free()
    {
        for (auto& asset : m_Assets)
        {
            delete asset.second;
        }
        m_Assets.clear();
    }

    void AssetRegistry::Serialize(const std::string& path)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;
            for (auto& pair : m_Assets)
            {
                out << YAML::BeginMap;
                {
                    Asset* asset = pair.second;
                    out << YAML::Key << asset->GetType() << YAML::Value << asset->GetID();
                    out << YAML::Key << "Name" << YAML::Value << asset->GetName();
                    out << YAML::Key << "Path" << YAML::Value << asset->GetPath();

                    // Not the best but it works
                    if (asset->GetType() == "Texture")
                    {
                        auto texture = dynamic_cast<Gfx::Texture*>(asset);
                        out << YAML::Key << "Type" << YAML::Value << texture->GetType();
                    }
                    if (asset->GetType() == "Model")
                    {
                        auto model = dynamic_cast<Gfx::Model*>(asset);
                        out << YAML::Key << "Material" << YAML::Value << model->GetMaterialHandle();
                    }
                    if (asset->GetType() == "Material")
                    {
                        auto material = dynamic_cast<Gfx::Material*>(asset);
                        out << YAML::Key << "DiffuseTexture" << YAML::Value << material->DiffuseTextureHandle;
                        out << YAML::Key << "SpecularTexture" << YAML::Value << material->SpecularTextureHandle;
                        out << YAML::Key << "NormalTexture" << YAML::Value << material->NormalTextureHandle;
                    }
                    if (asset->GetType() == "Font")
                    {
                        auto font = dynamic_cast<Gfx::Font*>(asset);
                        out << YAML::Key << "Size" << YAML::Value << font->GetSize();
                    }
                }
                out << YAML::EndMap;
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;

        // Write the result to file
        std::ofstream fout(path);
        fout << out.c_str();
        fout.close();
    }

    bool AssetRegistry::Deserialize(const std::string& path)
    {
        using namespace Core::Gfx;

        YAML::Node data;
        try
        {
            data = YAML::LoadFile(path);
        }
        catch (YAML::Exception e)
        {
            LOG_ERROR("Failed to load asset registry file", e.what());
            return false;
        }

        YAML::Node assetsNode = data["Assets"];
        if (!assetsNode || !assetsNode.IsSequence())
        {
            LOG_ERROR("Asset registry file is invalid or corrupted.");
            return false;
        }

        // Add empty assets to the registry
        for (auto assetNode : assetsNode)
        {
            // Access the asset type and its ID
            std::string assetType = assetNode.begin()->first.as<std::string>();
            uint64_t assetID = assetNode.begin()->second.as<uint64_t>();

            AssetMetadata metadata
            {
                assetNode["Name"].as<std::string>(),    // Access Name
                assetNode["Path"].as<std::string>(),    // Access Path
                assetID                                // Use the ID from the first key-value pair
            };

            // Handle different asset types
            if (assetType == "Texture")
            {
                Track(new Texture(metadata, assetNode["Type"].as<std::string>("texture_diffuse")));
            }
            else if (assetType == "Shader")
            {
                Track(new Shader(metadata));
            }
            else if (assetType == "Font")
            {
                Track(new Font(metadata, assetNode["Size"].as<int>()));
            }
            else if (assetType == "Model")
            {
                Track(new Model(metadata, assetNode["Material"].as<uint64_t>()));
            }
            else if (assetType == "Material")
            {
                Track(new Material(metadata,
                    assetNode["DiffuseTexture"].as<uint64_t>(),
                    assetNode["SpecularTexture"].as<uint64_t>(),
                    assetNode["NormalTexture"].as<uint64_t>())
                );
            }
        }

        return true;
    }

}