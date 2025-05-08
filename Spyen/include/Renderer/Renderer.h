#pragma once
#include "Core/Core.h"

namespace Spyen
{
    class SPYEN_API Renderer
    {
    public:
        static void Init();
        static void Shutdown();


        static void BeginScene();
        static void EndScene();
        static void Flush();

        static void SubmitQuad(const Vec2& pos, float scale, float rotation, const Color& color);
    };
}
