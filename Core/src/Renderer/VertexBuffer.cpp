#include "VertexBuffer.h"
#include "Renderer.h"

namespace Core::Gfx
{

    VertexBuffer::VertexBuffer(unsigned int size)
    {
        GLCall(glGenBuffers(1, &m_RendererID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW))
    }

    VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool dynamic_draw)
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

    void VertexBuffer::Free()
    {
        GLCall(glDeleteBuffers(1, &m_RendererID));
    }

    void VertexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    }

    void VertexBuffer::UnBind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void VertexBuffer::UpdateData(const void* data, int size)
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }

    void VertexBuffer::Resize(int size, bool dynamic_draw)
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        if (dynamic_draw)
        {
            GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
        }
        else
        {
            GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW));
        }
    }

}