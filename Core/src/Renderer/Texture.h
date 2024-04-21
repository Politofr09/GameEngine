#include <string>

#pragma once

namespace Core::Gfx 
{

	class Texture
	{
	private:
		unsigned int m_RendererID; // OpenGL ID

		int m_Width;
		int m_Height;
		int m_NrChannels;

		bool m_Loaded;

	public:

		Texture();
		Texture(const std::string& path);

		bool Load(const std::string& path);

		void Bind() const;
		void UnBind() const;
		bool IsLoaded() const;
	};

}