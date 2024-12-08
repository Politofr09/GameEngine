#include "Texture.h"
#include "Core/Utils.h"
#include <GL/glew.h>
#include <stb/stb_image.h>

#include "Core/Application.h"

namespace Core::Gfx
{

	Ref<Texture> Texture::Create(const std::string& path)
	{
		AssetMetadata metadata;
		metadata.Path = path;
		metadata.ID = UUID();
		metadata.Name = "Texture" + std::to_string(metadata.ID);
		metadata.Type = "Texture";
		return Texture::Create(metadata);
	}

	Ref<Texture> Texture::Create(const AssetMetadata& metadata)
	{
		Ref<Texture> texture = CreateRef<Texture>(metadata);
		texture->Load();
		return texture;
	}

	Ref<Texture> Texture::CreateFromMemory(unsigned int width, unsigned int height, int channels, void* data)
	{
		AssetMetadata metadata;
		metadata.ID = UUID();
		metadata.Name = "Texture" + std::to_string(metadata.ID);
		metadata.Type = "Texture";
		Ref<Texture> texture = CreateRef<Texture>(metadata);
		texture->LoadFromMemory(width, height, channels, data);
		return texture;
	}

	bool Texture::Load()
	{
		//stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(m_Metadata.Path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);

		if (data == NULL)
		{
			LOG_ERROR("stbi_load failed to load image: " + m_Metadata.Path);
			return m_Loaded = false;
		}

		LoadFromMemory(m_Width, m_Height, m_NrChannels, data);
		stbi_image_free(data);

		LOG_INFO("Texture " + m_Metadata.Name + " loaded.");

		return true;
	}

	bool Texture::UnLoad()
	{
		glDeleteTextures(1, &m_RendererID);
		return true;
	}

	void Texture::LoadFromMemory(unsigned int width, unsigned int height, int channels, void* data)
	{
		m_Width = width;
		m_Height = height;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	
		// Determine the texture format based on the number of channels
		GLenum format;
		switch (channels)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			LOG_ERROR("Unsupported number of channels: " + m_NrChannels);
			return;
		}

		// Set texture data
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenerateMipmap(GL_TEXTURE_2D);

		m_Loaded = true;
	}

	void Texture::Bind(uint32_t slot) const
	{
		//glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glBindTextureUnit(slot, m_RendererID);
	}

	void Texture::UnBind(uint32_t slot) const
	{
		//glBindTexture(GL_TEXTURE_2D, 0);
		glBindTextureUnit(slot, 0);
	}

}