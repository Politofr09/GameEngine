#include <glm/glm.hpp>
#include <map>
#include <string>
#include <stb/stb_truetype.h>

#pragma once

namespace Core::Gfx
{

	struct Character
	{
		unsigned int	TextureID;
		glm::ivec2		Size;
		glm::ivec2		Bearing;
		unsigned int	Advance;
		glm::vec2		TexCoords[2];
	};
	typedef Character Glyph;

	class Font
	{
	public:
		Font() = default;
		Font(const std::string& path, int fontSize);

		void Load(const std::string& path, int fontSize);
		Character GetCharacter(char c) const { return m_Characters.at(c); }
		int GetTextureID() const { return m_TextureID; }
		int GetSize() const { return m_Size; }
		void LoadFromMemory(unsigned char* fontData, int dataSize, int fontSize);

		glm::ivec2 MeasureText(const std::string& text, float scale) const;

	private:
		std::unordered_map<char, Character> m_Characters;
		stbtt_bakedchar m_BakedChars[128]{}; // ASCII 32..126 is 95 glyphs
		unsigned int m_TextureID;
		int m_Size = 0;
	};

}
