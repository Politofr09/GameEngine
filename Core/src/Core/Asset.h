#include "UUID.h"

#include <string>

#pragma once

namespace Core
{
// Similar to the DECLARE_EVENT_TYPE macro. Does the same but with assets!
#define DECLARE_ASSET_TYPE(asset) static constexpr DescriptorType descriptor = asset; \
								  virtual DescriptorType GetType() const override { return descriptor; }
    class Asset
    {
    public:
        // Default constructor
        Asset()
        {
            m_Loaded = false;
            m_FileDirectory = "";
            m_Name = "Asset-" + std::to_string(ID);
        }

        Asset& operator=(const Asset& other)
        {
            this->ID = other.ID;
            this->m_FileDirectory = other.m_FileDirectory;
            this->m_Name = other.m_Name;
            this->m_Loaded = other.m_Loaded;
            return *this;
        }

		using DescriptorType = const char*;

		virtual DescriptorType GetType() const = 0;

        const std::string& GetName() { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        const std::string& GetFileDirectory() { return m_FileDirectory; }

        bool IsLoaded() const { return m_Loaded; }
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