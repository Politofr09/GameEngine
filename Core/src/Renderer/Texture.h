#include <string>
#include "Core/Asset.h"

#pragma once

namespace Core::Gfx 
{

	struct TextureSpecifiation
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		int Channels = 3;
		bool Mipmaps = true;

		enum class Type
		{
			Texture2D,
			Cubemap
		} Type = Type::Texture2D;
	};

	class Texture
	{
	public:
		static Ref<Texture> Create(const std::string& path, const TextureSpecifiation& spec = TextureSpecifiation{});
		static Ref<Texture> Create(const AssetMetadata& metadata, const TextureSpecifiation& spec = TextureSpecifiation{});

		static Ref<Texture> CreateFromMemory(unsigned int width, unsigned int height, int channels, unsigned char* data);

		// Default constructor
		Texture() = default;
		Texture(const AssetMetadata& metadata, TextureSpecifiation spec)
			: m_Metadata(metadata), m_Spec(spec) {}

		Texture& operator=(const Texture& other)
		{
			this->m_Spec = other.m_Spec;
			this->m_RendererID = other.m_RendererID;
			this->m_Metadata = other.m_Metadata;

			return *this;
		}

		bool Load();
		bool LoadCubemap(unsigned char* data);
		void LoadFromMemory(unsigned char* data);

		bool UnLoad() const;

		void Bind(uint32_t slot = 0) const;
		void UnBind(uint32_t slot = 0) const;
		unsigned int GetID() const { return m_RendererID; }

		int GetWidth() const { return m_Spec.Width; }
		int GetHeight() const { return m_Spec.Height; }

		AssetMetadata& GetMetadata() { return m_Metadata; }
		bool IsLoaded() const { return m_Loaded; }

		TextureSpecifiation GetSpecification() const { return m_Spec; }

	private:
		unsigned int GetGLFormat();
	private:
		unsigned int m_RendererID = 0; // OpenGL ID
		TextureSpecifiation m_Spec;

		AssetMetadata m_Metadata;
		bool m_Loaded = false;
	};

}