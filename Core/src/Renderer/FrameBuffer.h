#include "Events/Event.h"

#pragma once

namespace Core::Events
{

	class ViewportResizedEvent : public Event
	{
	public:
		int width, height;

		ViewportResizedEvent(int width, int height) : width(width), height(height) {}

		DECLARE_EVENT_TYPE("ViewportResizedEvent");
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
		float GetAspectRatio() { return m_Width / m_Height;  }

		int GetOffsetX() const { return m_PositionX; }
		int GetOffsetY() const { return m_PositionY; }

		void SetPosition(int x, int y) { m_PositionX = x; m_PositionY = y; }

		void Free();

	private:
		unsigned int m_TextureID;
		unsigned int m_FrameBufferID;
		unsigned int m_RenderBufferID;

		unsigned int m_Width;
		unsigned int m_Height;
		int m_PositionX;
		int m_PositionY;
	};

}