#include <GL/glew.h>
#include "Core/Utils.h"
#include "VertexBuffer.h"
#include <vector>

#pragma once

namespace Core::Gfx
{

	struct BufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type);
	};

	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride;
		
	public:
		BufferLayout() : m_Stride(0) {}

		template <typename T>
		void Push(unsigned int count)
		{
			static_assert(false);
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
	};

	template<>
	void BufferLayout::Push<float>(unsigned int count);

	template<>
	void BufferLayout::Push<unsigned int>(unsigned int count);

	template<>
	void BufferLayout::Push<unsigned char>(unsigned int count);

	// TODO: Add more types?
	class VertexArray
	{
	private:
		unsigned int m_RendererID; // OpenGL ID
	
	public:
		VertexArray() = default;
		void Init();
		void Free();

		void AddBuffer(const VertexBuffer& vb, const BufferLayout& layout);
		void Bind() const;
		void UnBind() const;

		unsigned int GetRendererID() { return m_RendererID; }

		VertexArray& operator=(const VertexArray& other)
		{
			m_RendererID = other.m_RendererID;
			return *this;
		}
	};

}