#include "VertexBuffer.h"
#include "Renderer.h"

Core::Gfx::VertexBuffer::VertexBuffer(const void* data, int size, bool dynamic_draw)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    if (dynamic_draw)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
    }
    else
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
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

void Core::Gfx::VertexBuffer::UpdateData(const void* data, int size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}