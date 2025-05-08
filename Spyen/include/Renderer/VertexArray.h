#pragma once
#include <memory>

#include "IndexBuffer.h"
#include "Renderer/VertexBuffer.h"

namespace Spyen
{

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;
        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
        void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);
        uint32_t* GetRendererID();

        static std::shared_ptr<VertexArray> Create();

    private:
        uint32_t m_RendererID;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        uint32_t m_VertexBufferIndex = 0;
    };
}
