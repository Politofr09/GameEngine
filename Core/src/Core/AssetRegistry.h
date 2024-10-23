#include "Asset.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string.h>
#include <yaml-cpp/yaml.h>

#include "Core/Utils.h"

#pragma once

namespace Core
{
    typedef uint64_t AssetHandle;

    class AssetRegistry
    {
    public:
        AssetRegistry() = default;
        ~AssetRegistry() = default;

        AssetHandle Track(Asset* asset); // Add an asset to the list

        void Kill(Asset* asset); // Remove and free the asset
        void Free();

        /*std::vector<Asset*> GetAllResources() { return m_Assets; }*/
        std::unordered_map<AssetHandle, Asset*> GetAllResources() { return m_Assets; }

        //template<typename T>
        //T* GetFirstByName(const std::string& name)
        //{
        //    for (auto asset : m_Assets)
        //    {
        //        if (asset->GetName() == name)
        //        {
        //            if (std::string(asset->GetType()) == std::string(T::descriptor)) return dynamic_cast<T*>(asset);
        //        }
        //    }
        //    return nullptr;
        //}

        template <typename T>
        T* Get(AssetHandle handle)
        {
            // Check if handle is valid
            auto it = m_Assets.find(handle);
            if (it == m_Assets.end()) 
            {
                //LOG_ERROR("Asset with handle ", handle, " not found in registry. Returning dummy asset");
                return new T();
            }

            // Check if the asset is loaded (if not, load it)
            Asset* asset = it->second;
            if (!asset->IsLoaded())
            {
                // The asset can be reconstructed with its metadata
                asset->Load(); // load and set m_Loaded to true
            }

            // Finally, cast the asset and return it
            return dynamic_cast<T*>(asset);
        }

        bool Exists(AssetHandle handle)
        {
            return m_Assets.find(handle) != m_Assets.end();
        }

        bool Loaded(AssetHandle handle)
        {
            return m_Assets.find(handle)->second->IsLoaded();
        }

        template <typename T>
        AssetHandle FindByPath(const std::string& path)
        {
            for (auto& [handle, asset] : m_Assets)
            {
                // Check if the asset is of the desired type and if the path matches
                if (dynamic_cast<T*>(asset) && asset->GetPath() == path)
                {
                    return handle;
                }
            }
            return 0;
        }
    
        void Serialize(const std::string& path);
        bool Deserialize(const std::string& path);

    private:
        //std::vector<Asset*> m_Assets;
        std::unordered_map<AssetHandle, Asset*> m_Assets;
    };

}