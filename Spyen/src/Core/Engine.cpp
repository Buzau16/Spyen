//
// Created by gpro9 on 5/7/2025.
//

#include <Core/Engine.h>

#include "Core/Log.h"
#include "Window/Window.h"

namespace Spyen {
    struct EngineData
    {
        Window window;
    };
    static EngineData s_EngineData;

    void Engine::Init(uint32_t width, uint32_t height, const char* title)
    {
        Log::Init();
        s_EngineData.window.Init(width, height, title);

    }

    void Engine::Run()
    {
        while (s_EngineData.window.IsOpen())
        {
            s_EngineData.window.PollEvents();

            s_EngineData.window.SwapBuffers();

        }
        s_EngineData.window.Close();

    }
} // Spyen