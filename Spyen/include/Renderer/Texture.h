#pragma once
#include <Core/Core.h>
#include <memory>
#include <filesystem>

namespace Spyen {
    struct TextureSpecs {
        int Width = 1, Height = 1;
        int32_t Channels = 4;
        unsigned int Format = 0x1908;
    };


    class SPYEN_API Texture {
    public:
        Texture() = default;
        Texture(const std::filesystem::path& path);
        Texture(const TextureSpecs& specs);
        Texture(const unsigned char *data, uint32_t width, uint32_t height, uint8_t channels);

        void Bind(uint32_t slot) const;
        void Unbind(uint32_t slot) const;
        void SetData(void* data);

        [[nodiscard]] uint32_t GetWidth() const { return m_Specs.Width; }
        [[nodiscard]] uint32_t GetHeight() const { return m_Specs.Height; }
        [[nodiscard]] uint32_t GetRendererID() const { return m_RendererID; }

        static std::shared_ptr<Texture> Create(const std::filesystem::path& path);
        static std::shared_ptr<Texture> Create(const TextureSpecs& specs);
        // for prototyping
        static std::shared_ptr<Texture> Create(const unsigned char *data, uint32_t width, uint32_t height, uint8_t channels);

        bool operator==(const Texture& other) const
        {
            return this->m_RendererID == other.GetRendererID();
        }

    private:
        uint32_t m_RendererID;
        unsigned int m_InternalFormat;
        unsigned int m_DataFormat;
        TextureSpecs m_Specs;
    };
}
