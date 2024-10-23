#include "UUID.h"

#include <string>
#include <yaml-cpp/yaml.h>

#pragma once

namespace Core
{
// Similar to the DECLARE_EVENT_TYPE macro. Does the same but with assets!
#define DECLARE_ASSET_TYPE(asset) static constexpr DescriptorType descriptor = asset; \
								  virtual DescriptorType GetType() const override { return descriptor; }

    // Look at it like an intermediary representation of an asset.
    // When we load from disk, an empty asset gets created with the required metadata.
    // When we access the asset, if it's not loaded, we look at the metadata and reconstruct the asset
    struct AssetMetadata
    {
        std::string Name;
        std::string Path;
        UUID ID;
    };

    class Asset
    {
    public:
        // Default constructor
        Asset()
        {
            m_Loaded = false;
            //name --> "Asset-" + std::to_string();
        }

        Asset(const AssetMetadata& metadata)
        {
            m_Metadata = metadata;
        }

        Asset& operator=(const Asset& other)
        {
            this->m_Metadata = other.m_Metadata;
            return *this;
        }

		using DescriptorType = const char*;

		virtual DescriptorType GetType() const = 0;

        //virtual void Serialize(YAML::Emitter& out) = 0;
        ////virtual void Deserialize(YAML::Node& node) = 0;

        const std::string& GetName() { return m_Metadata.Name; }
        void SetName(const std::string& name) { m_Metadata.Name = name; }

        const std::string& GetPath() { return m_Metadata.Path; }

        bool IsLoaded() const { return m_Loaded; }

        UUID GetID() { return m_Metadata.ID; }

    protected:
        virtual bool Load() = 0;
        virtual bool UnLoad() = 0;

    protected:
        AssetMetadata m_Metadata;

        bool m_Loaded = false;
        // size_t m_MemoryUsage; // Don't know if this will be used

        friend class AssetRegistry;
    };

}