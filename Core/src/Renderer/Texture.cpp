#include "Texture.h"
#include "Core/Utils.h"
#include <GL/glew.h>
#include <stb/stb_image.h>

Core::Gfx::Texture::Texture()
{
	m_Width = 0;
	m_Height = 0;
	m_NrChannels = 0;
	m_RendererID = 0;
	m_Loaded = false;
	m_Type = "texture_diffuse";
	m_Path = "";
}

Core::Gfx::Texture::Texture(const std::string& path, std::string type)
{
	m_Type = type;
	Load(path);
}

Core::Gfx::Texture::Texture(const Texture& texture)
{
	*this = texture;
}

bool Core::Gfx::Texture::Load(const std::string& path)
{
	m_Path = path;
	
	//stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);

	if (data == NULL)
	{
		LOG_ERROR("stbi_load failed to load image: " + path);
		m_Loaded = false;
		return false;
	}

	LoadFromMemory(m_Width, m_Height, m_NrChannels, data);
	stbi_image_free(data);
	return true;
}

void Core::Gfx::Texture::LoadFromMemory(unsigned int width, unsigned int height, int channels, void* data)
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

void Core::Gfx::Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Core::Gfx::Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Core::Gfx::Texture::IsLoaded() const
{
	return m_Loaded;
}