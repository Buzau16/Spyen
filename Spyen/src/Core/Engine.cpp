//
// Created by gpro9 on 5/7/2025.
//

#include <iostream>
#include <Core/Engine.h>

#include "Core/Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Time/Timestep.h"
#include "Window/Window.h"

namespace Spyen {
    struct EngineData
    {
        Spyen::Window Window;
        Color BackgroundColor;
        std::shared_ptr<Scene> ActiveScene;
        std::unordered_map<std::string, std::shared_ptr<Scene>> Scenes;
    };
    static EngineData s_EngineData;

    void Engine::Init(uint32_t width, uint32_t height, const char* title)
    {
        Log::Init();
        s_EngineData.Window.Init(width, height, title);
        Renderer::Init();
    }

    void Engine::SetVsync(bool value) {
        s_EngineData.Window.SetVsync(value);
    }

    void Engine::SetBackgroundColor(const float r, const float g, const float b, const float a)
    {
        s_EngineData.BackgroundColor = {r,g,b,a};
    }

    weak_ptr<Scene> Engine::CreateScene(const std::string &name) {
        if (s_EngineData.Scenes.contains(name)) {
            SPY_CORE_WARN("Scene {} already exist!", name);
            return weak_ptr<Scene>(s_EngineData.Scenes[name]);
        }
        s_EngineData.Scenes[name] = std::make_shared<Scene>();
        return weak_ptr<Scene>(s_EngineData.Scenes[name]);
    }

    void Engine::SetActiveScene(const std::string &name) {
        if (!s_EngineData.Scenes.contains(name)) {
            SPY_CORE_ERROR("Scene {} does not exist!", name);
        }

        s_EngineData.ActiveScene = s_EngineData.Scenes[name];
    }

    weak_ptr<Scene> Engine::GetSceneByName(const std::string &name) {
        if (!s_EngineData.Scenes.contains(name)) {
            SPY_CORE_ERROR("Scene {} does not exist!", name);
            return weak_ptr<Scene>();
        }
        return weak_ptr<Scene>(s_EngineData.Scenes[name]);
    }

    void Engine::Run()
    {
        float LastFrameTime = 0.0f;

        while (s_EngineData.Window.IsOpen())
        {
            s_EngineData.Window.PollEvents();

            const auto time = static_cast<float>(glfwGetTime());
            Timestep ts(time - LastFrameTime);
            LastFrameTime = time;

            s_EngineData.Window.Clear(s_EngineData.BackgroundColor.r, s_EngineData.BackgroundColor.g, s_EngineData.BackgroundColor.b, s_EngineData.BackgroundColor.a);

            s_EngineData.ActiveScene->RunSceneSystems(ts);
            s_EngineData.ActiveScene->RenderScene();

            s_EngineData.Window.SwapBuffers();

        }
        s_EngineData.Window.Close();
        Renderer::Shutdown();

    }
} // Spyen