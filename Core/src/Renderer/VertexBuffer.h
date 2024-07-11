#pragma once

namespace Core::Gfx
{

	class VertexBuffer
	{
	private:
		unsigned int m_RendererID; // OpenGL ID

	public:
		VertexBuffer() = default;
		VertexBuffer(const void* data, int size, bool dynamic_draw = false);
		void Free();

		void UpdateData(const void* data, int size);

		void Bind() const;
		void UnBind() const;

		VertexBuffer& operator=(const VertexBuffer& other)
		{
			m_RendererID = other.m_RendererID;
			return *this;
		}
	};

}
