#pragma once

namespace Core::Gfx
{

	class VertexBuffer
	{
	private:
		unsigned int m_RendererID; // OpenGL ID

	public:
		VertexBuffer() = default;
		VertexBuffer(const void* data, int size);
		void Free();

		void Bind() const;
		void UnBind() const;
	};

}
