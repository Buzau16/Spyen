#include "Renderer/VertexBuffer.h"
#include <glad/glad.h>

namespace Spyen
{
    VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void VertexBuffer::UnBind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::SetData(const void* data, unsigned int size)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void VertexBuffer::SetLayout(const BufferLayout& layout)
    {
        m_Layout = layout;
    }

    BufferLayout VertexBuffer::GetLayout() const
    {
        return m_Layout;
    }

    uint32_t* VertexBuffer::GetRendererID()
    {
        return &m_RendererID;
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* data, unsigned int size)
    {
        return std::make_shared<VertexBuffer>(data, size);
    }
}
