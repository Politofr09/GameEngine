#pragma once

#include <string>

namespace Core::Gfx 
{

	class Texture
	{
	public:
		Texture();
		Texture(const std::string& path, std::string type = "texture_diffuse");
		Texture(const Texture& texture);

		Texture& operator=(const Texture& other)
		{
			this->m_Width = other.m_Width;
			this->m_Height = other.m_Height;
			this->m_NrChannels = other.m_NrChannels;
			this->m_RendererID = other.m_RendererID;
			this->m_Loaded = other.m_Loaded;
			this->m_Type = other.m_Type;
			this->m_Path = other.m_Path;

			return *this;
		}

		bool Load(const std::string& path);
		void LoadFromMemory(unsigned int width, unsigned int height, int channels, void* data);

		void Bind() const;
		void UnBind() const;
		bool IsLoaded() const;
		unsigned int GetID() { return m_RendererID; }

		std::string GetType() { return m_Type; }
		void SetType(std::string type) { m_Type = type; }

		std::string GetPath() { return m_Path; }

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }
		
	private:
		unsigned int m_RendererID; // OpenGL ID

		int m_Width;
		int m_Height;
		int m_NrChannels;

		bool m_Loaded;
		std::string m_Type;
		std::string m_Path;

	};

}