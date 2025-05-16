//
// Created by gpro9 on 5/7/2025.
//

#include <Core/Engine.h>

#include "Core/Log.h"
#include "glad/glad.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Window/Window.h"

namespace Spyen {
    struct EngineData
    {
        Window window;
        Color BackgroundColor;
        std::shared_ptr<Scene> ActiveScene;
    };
    static EngineData s_EngineData;

    void Engine::Init(uint32_t width, uint32_t height, const char* title)
    {
        Log::Init();
        s_EngineData.window.Init(width, height, title);
        Renderer::Init();
    }

    void Engine::SetBackgroundColor(const float r, const float g, const float b, const float a)
    {
        s_EngineData.BackgroundColor = {r,g,b,a};
    }

    void Engine::SetActiveScene(const std::shared_ptr<Scene>& scene) {
        s_EngineData.ActiveScene = scene;
    }

    void Engine::Run()
    {
        while (s_EngineData.window.IsOpen())
        {
            s_EngineData.window.PollEvents();

            s_EngineData.window.Clear(s_EngineData.BackgroundColor.r, s_EngineData.BackgroundColor.g, s_EngineData.BackgroundColor.b, s_EngineData.BackgroundColor.a);

            s_EngineData.ActiveScene->RenderScene();

            s_EngineData.window.SwapBuffers();

        }
        s_EngineData.window.Close();
        Renderer::Shutdown();

    }
} // Spyen