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

Core::Gfx::VertexArray::VertexArray()
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

void Core::Gfx::VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void Core::Gfx::VertexArray::UnBind() const
{
    glBindVertexArray(0);
}
