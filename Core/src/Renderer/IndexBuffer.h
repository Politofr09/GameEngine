#pragma once

namespace Core::Gfx
{

	class IndexBuffer
	{
	private:
		unsigned int m_RendererID; // OpenGL ID
		unsigned int m_Count;

	public:
		IndexBuffer() = default;
		IndexBuffer(const unsigned int* data, unsigned int count);
		void Free();

		void Bind() const;
		void UnBind() const;

		inline unsigned int GetCount() const { return m_Count; }
	};

}
