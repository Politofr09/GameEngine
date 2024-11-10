#include "VertexArray.h"

unsigned int Core::Gfx::BufferElement::GetSizeOfType(unsigned int type)
{
    switch (type)
    {
    case GL_FLOAT:          return 4; // type -> float
    case GL_UNSIGNED_INT:   return 4; // type -> unsigned int
    case GL_UNSIGNED_BYTE:  return 1; // type -> unsigned char
    }
    ASSERT(false);
    return 0;
}

void Core::Gfx::VertexArray::Init()
{
    glGenVertexArrays(1, &m_RendererID);
}

void Core::Gfx::VertexArray::Free()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void Core::Gfx::VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout)
{
    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
        offset += element.count * BufferElement::GetSizeOfType(element.type);
    }
}

void Core::Gfx::VertexArray::SetIndexBuffer(const Core::Gfx::IndexBuffer& ib)
{
    glBindVertexArray(m_RendererID);
    ib.Bind();
    m_IndexBuffer = ib;
}

void Core::Gfx::VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
    m_IndexBuffer.Bind();
}

void Core::Gfx::VertexArray::UnBind() const
{
    glBindVertexArray(0);
}

template<>
void Core::Gfx::BufferLayout::Push<float>(unsigned int count)
{
	m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
	m_Stride += count * BufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void Core::Gfx::BufferLayout::Push<unsigned int>(unsigned int count)
{
	m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	m_Stride += count * BufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void Core::Gfx::BufferLayout::Push<unsigned char>(unsigned int count)
{
	m_Elements.push_back({ GL_BYTE, count, GL_FALSE });
	m_Stride += count * BufferElement::GetSizeOfType(GL_BYTE);
}