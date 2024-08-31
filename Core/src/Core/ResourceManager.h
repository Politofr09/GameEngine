#include "Resource.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string.h>

#pragma once

namespace Core
{

    class ResourceManager
    {
    public:
        static ResourceManager& get()
        {
            static ResourceManager instance;
            return instance;
        }

#define TRACK_RESOURCE(resource) ResourceManager::get().Track(resource)
        void Track(Resource* resource); // Add a resource to the list

#define KILL_RESOURCE(resource) ResourceManager::get().Kill(resource)
        void Kill(Resource* resource); // Remove and free the resource

        void Free();

        std::vector<Resource*> GetAllResources() { return m_Resources; }

        template<typename T>
        T* GetFirstByName(const std::string& name)
        {
            for (auto resource : m_Resources)
            {
                if (resource->GetName() == name)
                {
                    if (std::string(resource->GetType()) == std::string(T::descriptor)) return dynamic_cast<T*>(resource);
                }
            }
            return nullptr;
        }

    private:
        ResourceManager() = default;
        ~ResourceManager() = default;

        std::vector<Resource*> m_Resources;
    };

}