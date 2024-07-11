#include "Events/Event.h"

#pragma once

namespace Core::Events
{

	class RenderingAreaUpdatedEvent : public Event
	{
	public:
		int width, height;

		RenderingAreaUpdatedEvent(int width, int height) : width(width), height(height) {}

		DECLARE_EVENT_TYPE("RenderingAreaUpdatedEvent");
	};

}

namespace Core::Gfx
{

	class FrameBuffer
	{
	public:
		FrameBuffer() = default;
		FrameBuffer(unsigned int width, unsigned int height);

		void Create(unsigned int width, unsigned int height);
		void Resize(unsigned int width, unsigned int height);

		void Bind() const;
		void UnBind() const;

		unsigned int GetTextureID() const { return m_TextureID; }
		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		void Free();

	private:
		unsigned int m_TextureID;
		unsigned int m_FrameBufferID;
		unsigned int m_RenderBufferID;

		unsigned int m_Width;
		unsigned int m_Height;
	};

}