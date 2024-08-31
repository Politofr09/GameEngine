#include "ResourceManager.h"
#include "Utils.h"
#include <algorithm>

namespace Core
{

    void ResourceManager::Track(Resource* resource)
    {
        LOG_INFO(resource->GetName() + "(" + std::to_string(resource->ID) + ")" + " added to resource tracking list.");
        m_Resources.push_back(resource);
    }

    void ResourceManager::Kill(Resource* resource)
    {
        auto it = std::find(m_Resources.begin(), m_Resources.end(), resource);
        if (it != m_Resources.end())
        {
            delete resource;
            m_Resources.erase(it);
        }
    }

    void ResourceManager::Free()
    {
        for (auto resource : m_Resources)
        {
            delete resource;
        }
        m_Resources.clear();
    }
}