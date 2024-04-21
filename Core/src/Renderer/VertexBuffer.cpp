#include "VertexBuffer.h"
#include "Renderer.h"

Core::Gfx::VertexBuffer::VertexBuffer(const void* data, int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void Core::Gfx::VertexBuffer::Free()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void Core::Gfx::VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void Core::Gfx::VertexBuffer::UnBind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
