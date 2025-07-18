#include "Renderer/IndexBuffer.h"
#include <glad/glad.h>

namespace Spyen
{
    IndexBuffer::IndexBuffer(const uint32_t* indices, const uint32_t count) : m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t IndexBuffer::GetCount() const
    {
        return m_Count;
    }

    uint32_t* IndexBuffer::GetRendererID()
    {
        return &m_RendererID;
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        return std::make_shared<IndexBuffer>(indices, count);
    }
}
