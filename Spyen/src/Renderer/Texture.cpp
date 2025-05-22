#include <Renderer/Texture.h>

#include "Core/LogMacros.h"
#include "glad/glad.h"
#include "Renderer/stb_image.h"

namespace Spyen {
    Texture::Texture(const std::filesystem::path &path) {
        SPY_CORE_INFO("Loading texture at {}", path.string());
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &m_Specs.Width, &m_Specs.Height, &m_Specs.Channels, 0);
        if (!data) {
            SPY_CORE_CRITICAL("Failed to load texture {} into ram.", path.string());
            return;
        }

        m_InternalFormat = m_Specs.Channels == 4 ? GL_RGBA8 : GL_RGB8;
        m_DataFormat = m_Specs.Channels == 4 ? GL_RGBA : GL_RGB;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Specs.Width, m_Specs.Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specs.Width, m_Specs.Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

        GLenum error = glGetError();
        SPY_CORE_ERROR("Error in Texture creation with code: {0}", error);

        stbi_image_free(data);
    }

    Texture::Texture(const TextureSpecs &specs) {
        m_Specs = specs;
        m_InternalFormat = (m_Specs.Channels == 4) ? GL_RGB8 : GL_RGBA8;
        m_DataFormat = (m_Specs.Channels == 4) ? GL_RGB : GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Specs.Width, m_Specs.Height);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void Texture::Bind(uint32_t slot) const {
        glBindTextureUnit(slot, m_RendererID);
    }

    void Texture::Unbind(uint32_t slot) const {
        glBindTextureUnit(slot, 0);
    }

    void Texture::SetData(void *data) {
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specs.Width, m_Specs.Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    std::shared_ptr<Texture> Texture::Create(const std::filesystem::path &path) {
        return std::make_shared<Texture>(path);
    }

    std::shared_ptr<Texture> Texture::Create(const TextureSpecs &specs) {
        return std::make_shared<Texture>(specs);
    }
}
