#include "Base.h"
#include "AssetMetadata.h"

#include <string.h>
#include <iterator>
#include <filesystem>
#include <queue>

#pragma once

namespace Core
{

    template <typename Asset>
    class AssetStorage
    {
    public:
        AssetStorage() = default;
        ~AssetStorage() = default;

        UUID Add(const AssetMetadata& metadata, Ref<Asset> asset)
        {
            m_Assets[metadata] = std::move(asset);
            return metadata.ID;
        }

        UUID Add(UUID ID, Ref<Asset> asset)
        {
            for (auto& [metadata, existingAsset] : m_Assets) 
            {
                if (metadata.ID == ID) 
                {
                    existingAsset = std::move(asset);
                    return ID;
                }
            }
            // Create a new metadata entry if not found
            AssetMetadata newMetadata;
            newMetadata.ID = ID;
            m_Assets[newMetadata] = std::move(asset);
            return ID;
        }

        // TODO: void Kill(const UUID& ID);
        //       void Kill(Ref<Asset> asset);

        Ref<Asset> Get(const AssetMetadata& metadata)
        {
            auto it = m_Assets.find(metadata);
            if (it != m_Assets.end())
            {
                return it->second;
            }

            return CreateRef<Asset>();
        }
        
        Ref<Asset> Get(UUID ID)
        {
            for (auto [metadata, asset] : m_Assets)
            {
                if (metadata.ID == ID)
                {
                    return asset;
                }
            }
            return CreateRef<Asset>();
        }

        Ref<Asset> Get(const std::string& path)
        {
            std::filesystem::path inputPath = std::filesystem::weakly_canonical(path); // Resolve '..' and '.'
            for (const auto& [metadata, asset] : m_Assets)
            {
                std::filesystem::path assetPath = std::filesystem::weakly_canonical(metadata.Path);
                if (inputPath == assetPath)
                {
                    return asset;
                }
            }
            return CreateRef<Asset>();
        }

        bool Exists(const AssetMetadata& metadata)
        {
            return m_Assets.find(metadata) != m_Assets.end();
        }

        bool Exists(UUID ID)
        {
            return Exists(AssetMetadata(ID));
        }

        void Free()
        {
            m_Assets.clear(); // Smart pointers will delete when going out of scope
        }

        // Fancy syntax support
        using Iterator = typename std::unordered_map<AssetMetadata, Ref<Asset>>::iterator;
        using ConstIterator = typename std::unordered_map<AssetMetadata, Ref<Asset>>::const_iterator;

        Iterator begin() { return m_Assets.begin(); }
        Iterator end() { return m_Assets.end(); }
        ConstIterator begin() const { return m_Assets.begin(); }
        ConstIterator end() const { return m_Assets.end(); }
        ConstIterator cbegin() const { return m_Assets.cbegin(); }
        ConstIterator cend() const { return m_Assets.cend(); }

        Ref<Asset>& operator[](const AssetMetadata& metadata)
        {
            auto it = m_Assets.find(metadata);
            if (it == m_Assets.end())
            {
                Ref<Asset> newAsset = CreateRef<Asset>();
                m_Assets[metadata] = newAsset;
                return m_Assets[metadata];
            }
            return it->second;
        }

    private:
        std::unordered_map<AssetMetadata, Ref<Asset>> m_Assets;
    };

    // Forward declaration
    namespace Gfx
    {
        class Texture;
        class Model;
        class Material;
        class Shader;
        class Font;
    }

    class AssetRegistry
    {
    public:
        AssetStorage<Gfx::Texture> GetTextureStorage()   { return m_Textures;  }
        AssetStorage<Gfx::Model> GetModelStorage()       { return m_Models;    }
        AssetStorage<Gfx::Material> GetMaterialStorage() { return m_Materials; }
        AssetStorage<Gfx::Shader> GetShaderStorage()     { return m_Shaders;   }
        AssetStorage<Gfx::Font> GetFontStorage()         { return m_Fonts;     }

        template <typename T>
        UUID Track(Ref<T> asset)
        {
            static_assert(false, "Unsupported asset type for Track<T>(). Specialize Track<T>() for this asset type.");
            return 0;
        };

        template <typename T>
        Ref<T> Get(const AssetMetadata& metadata)
        {
            static_assert(false, "Unsupported asset type for Get<T>(). Specialize Get<T>() for this asset type.");
            return Ref<T>();
        };

        template <typename T>
        Ref<T> Get(UUID id)
        {
            static_assert(false, "Unsupported asset type for Get<T>(). Specialize Get<T>() for this asset type.");
            return Ref<T>();
        }

        template <typename T>
        Ref<T> Get(const std::string& path)
        {
            static_assert(false, "Unsupported asset type for Get<T>(). Specialize Get<T>() for this asset type.");
            return Ref<T>();
        }


        template <typename T>
        bool Exists(UUID id)
        {
            return Exists<T>(AssetMetadata(id));
        }

        template <typename T>
        bool Exists(const AssetMetadata& metadata)
        {
            static_assert(false, "Unsupported asset type for Exists<T>(). Specialize Exists<T>() for this asset type.");
            return false;
        }

        // Write metadata to multiple **.metadata files in YAML format
        // .texture.metadata
        // .model.metadata
        // etc...
        void Serialize();

        // Populate
        void Deserialize();
        std::vector<std::filesystem::path> Scan() const;

        // void Rescan() <-- hot reloading... just sayin' ...

        void SetAssetDirectory(const std::string& directory) { m_AssetDirectory = directory + "/"; }
        std::string GetAssetDirectory() { return m_AssetDirectory; }
        void Free();

        void SerializeMetadata(const AssetMetadata& metadata);
        AssetMetadata DeserializeMetadata(const std::string& filename);
    private:
        AssetStorage<Gfx::Texture> m_Textures;
        AssetStorage<Gfx::Model> m_Models;
        AssetStorage<Gfx::Material> m_Materials;
        AssetStorage<Gfx::Shader> m_Shaders;
        AssetStorage<Gfx::Font> m_Fonts;

        std::string m_AssetDirectory = "res/";
    };

    #pragma region Track
    template <>
    UUID AssetRegistry::Track<Gfx::Texture>(Ref<Gfx::Texture> texture);

    template <>
    UUID AssetRegistry::Track<Gfx::Model>(Ref<Gfx::Model> model);

    template <>
    UUID AssetRegistry::Track<Gfx::Material>(Ref<Gfx::Material> material);

    template <>
    UUID AssetRegistry::Track<Gfx::Shader>(Ref<Gfx::Shader> shader);

    template <>
    UUID AssetRegistry::Track<Gfx::Font>(Ref<Gfx::Font> font);
    #pragma endregion
    #pragma region Get
    template <>
    Ref<Gfx::Texture> AssetRegistry::Get<Gfx::Texture>(const AssetMetadata& metadata);
    template <>
    Ref<Gfx::Texture> AssetRegistry::Get<Gfx::Texture>(UUID ID);
    template <>
    Ref<Gfx::Texture> AssetRegistry::Get<Gfx::Texture>(const std::string& path);

    template <>
    Ref<Gfx::Model> AssetRegistry::Get<Gfx::Model>(const AssetMetadata& metadata);
    template <>
    Ref<Gfx::Model> AssetRegistry::Get<Gfx::Model>(UUID ID);
    template <>
    Ref<Gfx::Model> AssetRegistry::Get<Gfx::Model>(const std::string& path);

    template <>
    Ref<Gfx::Material> AssetRegistry::Get<Gfx::Material>(const AssetMetadata& metadata);
    template <>
    Ref<Gfx::Material> AssetRegistry::Get<Gfx::Material>(UUID ID);
    template <>
    Ref<Gfx::Material> AssetRegistry::Get<Gfx::Material>(const std::string& path);

    template <>
    Ref<Gfx::Shader> AssetRegistry::Get<Gfx::Shader>(const AssetMetadata& metadata);
    template <>
    Ref<Gfx::Shader> AssetRegistry::Get<Gfx::Shader>(UUID ID);
    template <>
    Ref<Gfx::Shader> AssetRegistry::Get<Gfx::Shader>(const std::string& path);

    template <>
    Ref<Gfx::Font> AssetRegistry::Get<Gfx::Font>(const AssetMetadata& metadata);
    template <>
    Ref<Gfx::Font> AssetRegistry::Get<Gfx::Font>(UUID ID);
    template <>
    Ref<Gfx::Font> AssetRegistry::Get<Gfx::Font>(const std::string& path);
    #pragma endregion
    #pragma region Exists
    template <>
    bool AssetRegistry::Exists<Gfx::Texture>(const AssetMetadata& metadata);

    template <>
    bool AssetRegistry::Exists<Gfx::Model>(const AssetMetadata& metadata);

    template <>
    bool AssetRegistry::Exists<Gfx::Material>(const AssetMetadata& metadata);

    template <>
    bool AssetRegistry::Exists<Gfx::Shader>(const AssetMetadata& metadata);

    template <>
    bool AssetRegistry::Exists<Gfx::Font>(const AssetMetadata& metadata);
    #pragma endregion

}