#include <string>
#include "Core/Asset.h"
#include "Core/AssetRegistry.h"

#pragma once

namespace Core::Gfx 
{

	class Texture : public Asset
	{
	public:
		DECLARE_ASSET_TYPE("Texture")

		static AssetHandle Create(const std::string& path, const std::string& name = "Texture", std::string type = "texture_diffuse");

		// Default constructor
		Texture() = default;
		
		// Metadata constructor (+ additional required texture type)
		Texture(const AssetMetadata& metadata, std::string type);

		Texture& operator=(const Texture& other)
		{
			this->m_Width = other.m_Width;
			this->m_Height = other.m_Height;
			this->m_NrChannels = other.m_NrChannels;
			this->m_RendererID = other.m_RendererID;
			this->m_Loaded = other.m_Loaded;
			this->m_Type = other.m_Type;

			return *this;
		}

		bool Load() override;
		bool UnLoad() override;
		void LoadFromMemory(unsigned int width, unsigned int height, int channels, void* data);

		void Bind(uint32_t slot = 0) const;
		void UnBind(uint32_t slot = 0) const;
		unsigned int GetID() const { return m_RendererID; }

		std::string GetType() { return m_Type; }
		void SetType(std::string type) { m_Type = type; }

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	private:
		unsigned int m_RendererID = 0; // OpenGL ID

		int m_Width = 0;
		int m_Height = 0;
		int m_NrChannels = 0;

		std::string m_Type = "texture_diffuse";
	};

}