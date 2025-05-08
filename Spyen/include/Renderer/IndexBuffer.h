#pragma once
#include <cstdint>
#include <memory>

namespace Spyen
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const uint32_t* indices, uint32_t count);
        ~IndexBuffer();
        void Bind() const;
        void Unbind() const;
        uint32_t GetCount() const;
        uint32_t* GetRendererID();
        static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}
