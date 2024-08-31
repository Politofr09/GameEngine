#include <string>
#include "Core/Resource.h"
#include "Core/ResourceManager.h"

#pragma once

namespace Core::Gfx 
{

	class Texture : public Resource
	{
	public:
		DECLARE_RESOURCE_TYPE("Texture")

		static Texture& Create(const std::string& path, const std::string& name = "Texture", std::string type = "texture_diffuse");

		Texture();
		// Texture(const Texture& texture);

		// Texture& operator=(const Texture& other)
		// {
		// 	this->m_Width = other.m_Width;
		// 	this->m_Height = other.m_Height;
		// 	this->m_NrChannels = other.m_NrChannels;
		// 	this->m_RendererID = other.m_RendererID;
		// 	this->m_Loaded = other.m_Loaded;
		// 	this->m_Type = other.m_Type;

		// 	return *this;
		// }

		bool Load() override;
		bool UnLoad() override;
		void LoadFromMemory(unsigned int width, unsigned int height, int channels, void* data);

		void Bind() const;
		void UnBind() const;
		unsigned int GetID() { return m_RendererID; }

		std::string GetType() { return m_Type; }
		void SetType(std::string type) { m_Type = type; }

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }
		
	private:
		Texture(const std::string& filename, const std::string& name, std::string type);

	private:
		unsigned int m_RendererID; // OpenGL ID

		int m_Width;
		int m_Height;
		int m_NrChannels;

		std::string m_Type;
	};

}