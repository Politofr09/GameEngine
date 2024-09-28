#include "Asset.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string.h>

#pragma once

namespace Core
{

    class AssetRegistry
    {
    public:
        AssetRegistry() = default;
        ~AssetRegistry() = default;

        void Track(Asset* asset); // Add an asset to the list

        void Kill(Asset* asset); // Remove and free the asset

        void Free();

        std::vector<Asset*> GetAllResources() { return m_Assets; }

        template<typename T>
        T* GetFirstByName(const std::string& name)
        {
            for (auto asset : m_Assets)
            {
                if (asset->GetName() == name)
                {
                    if (std::string(asset->GetType()) == std::string(T::descriptor)) return dynamic_cast<T*>(asset);
                }
            }
            return nullptr;
        }

    private:
        std::vector<Asset*> m_Assets;
    };

}