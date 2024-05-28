#include <string>

#pragma once

namespace Core::Gfx 
{

	typedef enum
	{
		TEXTURE_NONE = 0,
		TEXTURE_DIFFUSE,	    // Base color 
		TEXTURE_SPECULAR,		// Specular color of the material
		TEXTURE_AMBIENT,		// Ambient color of the material
		TEXTURE_EMISSION,		// Defines emissive properties of the material
		TEXTURE_NORMAL,			// Modify the normal vectors
		TEXTURE_HEIGHT,			// Displacement of the surface to fake geometry
		TEXTURE_NOISE			// Extra texture for noise and achieve effects

	} TextureType;

	class Texture
	{
	public:
		Texture();
		Texture(const std::string& path, TextureType type = TEXTURE_DIFFUSE);

		bool Load(const std::string& path);
		void LoadFromMemory(unsigned int width, unsigned int height, int channels, void* data);

		void Bind() const;
		void UnBind() const;
		bool IsLoaded() const;
		unsigned int GetID() { return m_RendererID; }

		TextureType GetType() { return m_Type; }
		void SetType(TextureType type) { m_Type = type; }

		std::string GetPath() { return m_Path; }

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

	private:
		unsigned int m_RendererID; // OpenGL ID

		int m_Width;
		int m_Height;
		int m_NrChannels;

		bool m_Loaded;
		TextureType m_Type;
		std::string m_Path;
	};

}