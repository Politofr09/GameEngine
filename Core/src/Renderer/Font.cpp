#include "Font.h"
#include <fstream>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "Application.h"

namespace Core::Gfx
{

	Ref<Font> Font::Create(const std::string& path)
	{
		AssetMetadata metadata;
		metadata.Path = path;
		metadata.ID = UUID();
		metadata.Name = "Font" + std::to_string(metadata.ID);
		metadata.Type = "Font";
		return Font::Create(metadata);
	}

	Ref<Font> Font::Create(const AssetMetadata& metadata)
	{
		Ref<Font> font = CreateRef<Font>(metadata);
		font->Load();
		return font;
	}

	bool Font::Load()
    {
		std::ifstream file(m_Metadata.Path, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			throw std::runtime_error("Failed, to open font file: " + m_Metadata.Path);
			return false;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<unsigned char> fontData(size);
		if (!file.read(reinterpret_cast<char*>(fontData.data()), size))
		{
			throw std::runtime_error("Failed to read font file: " + m_Metadata.Path);
			return false;
		}

		return LoadFromMemory(fontData.data(), size, m_Size);
	}

	bool Font::LoadFromMemory(unsigned char* fontData, int dataSize, int fontSize)
	{
		std::vector<unsigned char> tempBitmap(512 * 512);

		stbtt_BakeFontBitmap(fontData, 0, fontSize, tempBitmap.data(), 512, 512, 32, 96, m_BakedChars);

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, tempBitmap.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenerateMipmap(GL_TEXTURE_2D);

		for (unsigned char c = 32; c < 128; c++)
		{
			stbtt_aligned_quad q{};
			float xpos = 0.0f;
			float ypos = 0.0f;
			stbtt_GetBakedQuad(m_BakedChars, 512, 512, c - 32, &xpos, &ypos, &q, 1);

			Character ch{ 0 };
			ch.Size = glm::ivec2(q.x1 - q.x0, q.y1 - q.y0);
			ch.Bearing = glm::ivec2(q.x0, q.y0);
			ch.Advance = xpos;  // xpos has been updated to the next position
			ch.TexCoords[0] = glm::vec2(q.s0, q.t0);
			ch.TexCoords[1] = glm::vec2(q.s1, q.t1);

			m_Characters.insert({ c, ch });
		}
		
		m_Loaded = true;

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	bool Font::UnLoad()
	{
		return true;
	}

	glm::ivec2 Font::MeasureText(const std::string& text, float scale) const 
	{
		scale /= 15;
		float width = 0.0f;
		float maxHeight = 0.0f;

		for (const char& c : text)
		{
			if (m_Characters.find(c) != m_Characters.end()) 
			{
				const Character& ch = m_Characters.at(c);
				width += ch.Advance * scale; // Advance is in 1/64 pixels, convert to pixels
				float height = ch.Size.y * scale;
				if (height > maxHeight) 
				{
					maxHeight = height;
				}
			}
		}

		return glm::ivec2(static_cast<int>(width), static_cast<int>(maxHeight));
	}

}
