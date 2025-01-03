#include "Texture.h"
#include "Core/Utils.h"
#include <GL/glew.h>
#include <stb/stb_image.h>

#include "Core/Application.h"

namespace Core::Gfx
{

	Ref<Texture> Texture::Create(const std::string& path, const TextureSpecifiation& spec)
	{
		AssetMetadata metadata;
		metadata.Path = path;
		metadata.ID = UUID();
		metadata.Name = "Texture" + std::to_string(metadata.ID);
		metadata.Type = "Texture";
		return Texture::Create(metadata, spec);
	}

	Ref<Texture> Texture::Create(const AssetMetadata& metadata, const TextureSpecifiation& spec)
	{
		Ref<Texture> texture = CreateRef<Texture>(metadata, spec);
		texture->Load();
		return texture;
	}

	Ref<Texture> Texture::CreateFromMemory(unsigned int width, unsigned int height, int channels, unsigned char* data)
	{
		AssetMetadata metadata;
		metadata.ID = UUID();
		metadata.Name = "Texture" + std::to_string(metadata.ID);
		metadata.Type = "Texture";
		Ref<Texture> texture = CreateRef<Texture>(metadata, TextureSpecifiation{});
		texture->LoadFromMemory(data);
		return texture;
	}

	bool Texture::Load()
	{
		//stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(m_Metadata.Path.c_str(), (int*) &m_Spec.Width, (int*) &m_Spec.Height, &m_Spec.Channels, 0);

		if (data == NULL)
		{
			LOG_ERROR("stbi_load failed to load image: " + m_Metadata.Path);
			return m_Loaded = false;
		}

		if (m_Spec.Type == TextureSpecifiation::Type::Texture2D)
			LoadFromMemory(data);
		else
			LoadCubemap(data);

		stbi_image_free(data);

		LOG_INFO("Texture " + m_Metadata.Name + " loaded.");

		return true;
	}

	bool Texture::LoadCubemap(unsigned char* data)
	{
		int faceWidth = m_Spec.Width / 4;
		int faceHeight = m_Spec.Height / 3;

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		const int faceOffsets[6][2] = {
			{ 2, 1 }, // Positive X
			{ 0, 1 }, // Negative X
			{ 1, 0 }, // Positive Y
			{ 1, 2 }, // Negative Y
			{ 1, 1 }, // Positive Z
			{ 3, 1 }  // Negative Z
		};
		
		for (int face = 0; face < 6; face++)
		{
			int offsetX = faceOffsets[face][0] * faceWidth;
			int offsetY = faceOffsets[face][1] * faceHeight;

			unsigned char* faceData = new unsigned char[faceWidth * faceHeight * 4];
			size_t rowStride = m_Spec.Width * 4; // Full source image stride (width in bytes)
			size_t faceStride = faceWidth * 4;  // Per-face stride (in bytes)
			for (int y = 0; y < faceHeight; ++y)
			{
				memcpy(
					faceData + (y * faceStride),
					data + ((offsetY + y) * rowStride) + (offsetX * 4),
					faceStride
				);
			}

			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				0, GL_RGBA, faceWidth, faceHeight,
				0, GL_RGBA, GL_UNSIGNED_BYTE, faceData
			);

			delete[] faceData;
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return m_RendererID != 0;
	}

	bool Texture::UnLoad() const
	{
		glDeleteTextures(1, &m_RendererID);
		return true;
	}

	void Texture::LoadFromMemory(unsigned char* data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &m_RendererID);

		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	
		// Determine the texture format based on the number of channels
		GLenum format;
		switch (m_Spec.Channels)
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
			LOG_ERROR("Unsupported number of channels: " + m_Spec.Channels);
			return;
		}

		// Set texture data
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Spec.Width, m_Spec.Height, 0, format, GL_UNSIGNED_BYTE, data);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (m_Spec.Mipmaps) glGenerateMipmap(GL_TEXTURE_2D);

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