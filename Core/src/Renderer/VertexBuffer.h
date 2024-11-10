#pragma once

namespace Core::Gfx
{

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(unsigned int size);
		VertexBuffer(const void* data, unsigned int size, bool dynamic_draw = false);

		
		void Free();

		void UpdateData(const void* data, int size);
		void Resize(int size, bool dynamic_draw = false);

		void Bind() const;
		void UnBind() const;

		int GetRendererID() { return m_RendererID; }

		VertexBuffer& operator=(const VertexBuffer& other)
		{
			m_RendererID = other.m_RendererID;
			return *this;
		}
	private:
		unsigned int m_RendererID; // OpenGL ID

	};

}
