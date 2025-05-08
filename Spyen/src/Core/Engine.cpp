//
// Created by gpro9 on 5/7/2025.
//

#include <Core/Engine.h>

#include "Core/Log.h"
#include "glad/glad.h"
#include "Renderer/Renderer.h"
#include "Window/Window.h"

namespace Spyen {
    struct EngineData
    {
        Window Window;
        Color BackgroundColor;
    };
    static EngineData s_EngineData;

    void Engine::Init(uint32_t width, uint32_t height, const char* title)
    {
        Log::Init();
        s_EngineData.Window.Init(width, height, title);
        Renderer::Init();
    }

    void Engine::SetBackgroundColor(const float r, const float g, const float b, const float a)
    {
        s_EngineData.BackgroundColor = {r,g,b,a};
    }

    void Engine::Run()
    {
        while (s_EngineData.Window.IsOpen())
        {
            s_EngineData.Window.PollEvents();

            s_EngineData.Window.Clear(s_EngineData.BackgroundColor.r, s_EngineData.BackgroundColor.g, s_EngineData.BackgroundColor.b, s_EngineData.BackgroundColor.a);

            Renderer::BeginScene();

            Renderer::SubmitQuad({0.0f, 0.0f}, 1.0f, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f});

            Renderer::EndScene();

            s_EngineData.Window.SwapBuffers();

        }
        s_EngineData.Window.Close();

    }
} // Spyen