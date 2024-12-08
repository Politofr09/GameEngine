#include "Core/Asset.h"
#include "Core/Asset.h"
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
		static Ref<Font> Create(const std::string& path);
		static Ref<Font> Create(const AssetMetadata& metadata);

		Font() = default;
		Font(const AssetMetadata& metadata)
			: m_Metadata(metadata) {}

		bool Load();
		bool UnLoad();		

		Character GetCharacter(char c) const { return m_Characters.at(c); }
		int GetTextureID() const { return m_TextureID; }
		int GetSize() const { return m_Size; }
		bool LoadFromMemory(unsigned char* fontData, int dataSize, int fontSize);

		glm::ivec2 MeasureText(const std::string& text, float scale) const;

		AssetMetadata& GetMetadata() { return m_Metadata; }
		bool IsLoaded() const { return m_Loaded; }

	private:
		std::unordered_map<char, Character> m_Characters;
		stbtt_bakedchar m_BakedChars[128]{}; // ASCII 32..126 is 95 glyphs
		unsigned int m_TextureID = 0;
		int m_Size = 0;

		AssetMetadata m_Metadata;
		bool m_Loaded = false;
	};

}
