#pragma once
#include "Texture.h"
#include "Core/Core.h"
#include "Math/fwd.hpp"

namespace Spyen
{
    class SPYEN_API Renderer
    {
    public:
        static void Init();
        static void Shutdown();


        static void BeginBatch();
        static void EndBatch();
        static void Flush();

        static void SubmitQuad(const Vec2& pos, float scale, float rotation, const Color& color);
        static void SubmitQuad(const Vec2& pos, float scale, float rotation, const std::shared_ptr<Texture>& texture);
        static void SubmitQuad(const glm::mat4& transform, const Color& color);
        static void SubmitQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture);
    };
}
