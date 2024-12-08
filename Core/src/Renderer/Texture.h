#include <string>
#include "Core/Asset.h"

#pragma once

namespace Core::Gfx 
{

	class Texture
	{
	public:
		static Ref<Texture> Create(const std::string& path);
		static Ref<Texture> Create(const AssetMetadata& metadata);

		static Ref<Texture> CreateFromMemory(unsigned int width, unsigned int height, int channels, void* data);

		// Default constructor
		Texture() = default;
		Texture(const AssetMetadata& metadata)
			: m_Metadata(metadata) {}

		Texture& operator=(const Texture& other)
		{
			this->m_Width = other.m_Width;
			this->m_Height = other.m_Height;
			this->m_NrChannels = other.m_NrChannels;
			this->m_RendererID = other.m_RendererID;
			this->m_Metadata = other.m_Metadata;
			//this->m_Type = other.m_Type;

			return *this;
		}

		bool Load();
		bool UnLoad();
		void LoadFromMemory(unsigned int width, unsigned int height, int channels, void* data);

		void Bind(uint32_t slot = 0) const;
		void UnBind(uint32_t slot = 0) const;
		unsigned int GetID() const { return m_RendererID; }

		//std::string GetType() { return m_Type; }
		//void SetType(std::string type) { m_Type = type; }

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		AssetMetadata& GetMetadata() { return m_Metadata; }
		bool IsLoaded() const { return m_Loaded; }

	private:
		unsigned int m_RendererID = 0; // OpenGL ID
		int m_Width = 0;
		int m_Height = 0;
		int m_NrChannels = 0;
		//std::string m_Type = "texture_diffuse";

		AssetMetadata m_Metadata;
		bool m_Loaded = false;
	};

}