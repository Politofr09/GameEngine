#include "UUID.h"

#include <string>

#pragma once

namespace Core
{
// Similar to the DECLARE_EVENT_TYPE macro. Does the same but with assets!
#define DECLARE_RESOURCE_TYPE(resource) static constexpr DescriptorType descriptor = resource; \
								  virtual DescriptorType GetType() const override { return descriptor; }

    class Resource
    {
    public:
        // Default constructor
        Resource()
        {
            m_Loaded = false;
            m_FileDirectory = "";
            m_Name = "Resource-" + std::to_string(ID);
        }

		using DescriptorType = const char*;

		virtual DescriptorType GetType() const = 0;

        const std::string& GetName() { return m_Name; }
        const std::string& GetFileDirectory() { return m_FileDirectory; }

        bool IsLoaded() { return m_Loaded; }
        UUID ID;
        
    protected:
        virtual bool Load() = 0;
        virtual bool UnLoad() = 0;

    protected:
        std::string m_Name;
        std::string m_FileDirectory;
        bool m_Loaded;        
        // size_t m_MemoryUsage; // Don't know if this will be used        
    };

}