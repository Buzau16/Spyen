//
// Created by gpro9 on 5/8/2025.
//

#include <Renderer/Renderer.h>

#include <memory>

#include "Core/Core.h"
#include <Math/glm.hpp>
#include <Math/gtc/matrix_transform.hpp>

#include "glad/glad.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"

namespace Spyen
{
    struct QuadVertex
    {
        glm::vec2 Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        int TexIndex;
    };


    struct RendererData


    {
        static constexpr uint32_t MaxQuads = 10000;
        static constexpr uint32_t MaxVertices = MaxQuads * 4;
        static constexpr uint32_t MaxIndices = MaxQuads * 6;
        static constexpr uint32_t MaxTextureSlots = 32;

        std::shared_ptr<VertexArray> QuadVertexArray;
        std::shared_ptr<VertexBuffer> QuadVertexBuffer;
        std::shared_ptr<IndexBuffer> QuadIndexBuffer;
        std::shared_ptr<Shader> QuadShader;
        std::shared_ptr<Texture> WhiteTexture;

        QuadVertex* QuadVertexBufferBase;
        QuadVertex* QuadVertexBufferPtr;
        uint32_t QuadIndexCount;
        glm::vec4 QuadPositions[4];

        std::array<std::shared_ptr<Texture>, MaxTextureSlots> TextureSlots;
        uint32_t TextureIndex = 1;
    };

    static RendererData s_Data;

    void Renderer::Init()
    {
        SPY_CORE_INFO("Initializing Spyen::Renderer");

        s_Data.QuadVertexBufferBase = new QuadVertex[RendererData::MaxVertices];
        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexArray->Bind();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(nullptr, RendererData::MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->Bind();
        s_Data.QuadVertexBuffer->SetLayout({
                { ShaderDataType::Float2, "a_Position" },
                { ShaderDataType::Float4, "a_Color" },
                { ShaderDataType::Float2, "a_TexCoords"},
                { ShaderDataType::Int, "a_TexIndex"}
        });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
        s_Data.QuadPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

        uint32_t* indices = new uint32_t[RendererData::MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < RendererData::MaxIndices; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }
        s_Data.QuadIndexBuffer = IndexBuffer::Create(indices, RendererData::MaxIndices);
        s_Data.QuadVertexArray->AddIndexBuffer(s_Data.QuadIndexBuffer);
        delete[] indices;


        // Create the WhiteTexture;
        s_Data.WhiteTexture = Texture::Create(TextureSpecs{1,1, .Format=GL_RGBA});
        uint32_t Whitetx = 0xffffffff;
        s_Data.WhiteTexture->SetData(&Whitetx);
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;
        int32_t samplers[32];
        for (int32_t i = 0; i < RendererData::MaxTextureSlots; i++) {
            samplers[i] = i;
        }

        s_Data.QuadShader = Shader::Create("QuadShader", "Spyen/shaders/QuadShader.vert", "Spyen/shaders/QuadShader.frag");
        s_Data.QuadShader->Bind();
        s_Data.QuadShader->SetUniform1iv("u_Textures", RendererData::MaxTextureSlots, samplers);
    }

    void Renderer::Shutdown()
    {
        glDeleteVertexArrays(1, s_Data.QuadVertexArray->GetRendererID());
        glDeleteBuffers(1, s_Data.QuadVertexBuffer->GetRendererID());
        glDeleteBuffers(1, s_Data.QuadIndexBuffer->GetRendererID());

        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer::BeginBatch()
    {
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.QuadIndexCount = 0;
        s_Data.TextureIndex = 1;
    }

    void Renderer::EndBatch()
    {
        const GLsizeiptr size = reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, size);
        for (int32_t i = 0; i < s_Data.TextureIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);
        Flush();
    }

    void Renderer::Flush()
    {
        s_Data.QuadShader->Bind();
        s_Data.QuadVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::SubmitQuad(const Vec2& pos, const float scale, const float rotation, const Color& color)
    {

        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scale, scale, 1.0f));

        SubmitQuad(transform, color);
    }

    void Renderer::SubmitQuad(const Vec2 &pos, float scale, float rotation, const std::shared_ptr<Texture> &texture) {
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scale, scale, 1.0f));

        SubmitQuad(transform, texture);
    }

    void Renderer::SubmitQuad(const glm::mat4& transform, const Color &color) {

        if (s_Data.QuadIndexCount >= RendererData::MaxIndices) {
            EndBatch();
            Flush();
            BeginBatch();
        }

        glm::vec2 textCoords[4] = {
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f }
        };

        for (int i = 0; i < 4; i++) {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[i];
            s_Data.QuadVertexBufferPtr->Color = glm::vec4(color.r, color.g, color.b, color.a);
            s_Data.QuadVertexBufferPtr->TexCoords = textCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = 0;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
    }

    void Renderer::SubmitQuad(const glm::mat4 &transform, const std::shared_ptr<Texture> &texture) {

        glm::vec2 textCoords[4] = {
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f }
        };

        float textureIndex = 0.0f;

        if (s_Data.QuadIndexCount >= RendererData::MaxIndices) {
            EndBatch();
            Flush();
            BeginBatch();
        }

        for (uint32_t i = 0; i < s_Data.TextureIndex; i++) {
            if (*s_Data.TextureSlots[i] == *texture) {
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        if (s_Data.TextureIndex >= RendererData::MaxTextureSlots) {
            EndBatch();
            Flush();
            BeginBatch();
        }

        if (textureIndex == 0.0f) {
            textureIndex = static_cast<float>(s_Data.TextureIndex);
            s_Data.TextureSlots[s_Data.TextureIndex] = texture;
            s_Data.TextureIndex++;
        }
        //SPY_CORE_WARN("TexIndex: {}", textureIndex);

        for (int i = 0; i < 4; i++) {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[i];
            s_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            s_Data.QuadVertexBufferPtr->TexCoords = textCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr++;
        }
        s_Data.QuadIndexCount += 6;
    }
}
