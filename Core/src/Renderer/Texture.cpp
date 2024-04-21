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
}

Core::Gfx::Texture::Texture(const std::string& path)
{
	Load(path);
}

bool Core::Gfx::Texture::Load(const std::string& path)
{
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);

	if (data == NULL)
	{
		LOG_ERROR("stbi_load failed to load image: " + path);
		m_Loaded = false;
		return false;
	}

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_NrChannels == 4 ? GL_RGBA : GL_RGB, m_Width, m_Height, 0, m_NrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set parameters to determine how the texture wraps at edges
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	m_Loaded = true;
	return true;
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
