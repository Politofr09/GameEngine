#include "AssetRegistry.h"
#include "Utils.h"
#include <algorithm>

namespace Core
{

    void AssetRegistry::Track(Asset* asset)
    {
        LOG_INFO(asset->GetName() + "(" + std::to_string(asset->ID) + ")" + " added to asset tracking list.");
        m_Assets.push_back(asset);
    }

    void AssetRegistry::Kill(Asset* asset)
    {
        auto it = std::find(m_Assets.begin(), m_Assets.end(), asset);
        if (it != m_Assets.end())
        {
            delete asset;
            m_Assets.erase(it);
        }
    }

    void AssetRegistry::Free()
    {
        for (auto asset : m_Assets)
        {
            delete asset;
        }
        m_Assets.clear();
    }
}