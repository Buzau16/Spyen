#pragma once
#include <cstdint>

#include "BufferLayout.h"

namespace Spyen
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void UnBind() const;
        void SetData(const void* data, unsigned int size);
        void SetLayout(const BufferLayout& layout);
        BufferLayout GetLayout() const;
        uint32_t* GetRendererID();

        static std::shared_ptr<VertexBuffer> Create(const void* data, unsigned int size);

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };
}
