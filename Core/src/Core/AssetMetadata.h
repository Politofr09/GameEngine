#pragma once

#include "Base.h"
#include <functional>

namespace Core
{ 

    // Look at it like an intermediary representation of an asset
    // When we access the asset, if it's not loaded, we look at the metadata and reconstruct the asset
    struct AssetMetadata
    {
        std::string Name;
        std::string Path;
        std::string Type;
        UUID        ID = 0;

        // Some assets like models and materials need to hold reference to other assets
        // If they need some reference, they just ask it to the (already populated) registry
        // Hopefully we don't duplicate assets with this.
        // Example for material might be:
        // --- key ---------- metadata ---
        // - "Diffuse"  : [102983091803928]
        // - "Specular" : [688291367843001]
        // ...
        std::unordered_map<std::string, UUID> Dependencies;

        // Don't know if I keep it...
        /*AssetMetadata(UUID id)
            : ID(id) {
        }*/

        AssetMetadata() = default;
        AssetMetadata(const AssetMetadata&) = default;
        AssetMetadata& operator=(const AssetMetadata&) = default;

        bool operator==(const AssetMetadata& other) const 
        {
            return ID == other.ID; // Compare based on unique identifier
        }
    };
}

namespace std
{
    template <>
    struct hash<Core::AssetMetadata>
    {
        std::size_t operator()(const Core::AssetMetadata& metadata) const 
        {
            return std::hash<uint64_t>()(metadata.ID);
        }
    };
}