#include <string>
#include <Window/Window.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <Input/Input.h>

#include "Core/LogMacros.h"

namespace Spyen {
    struct WindowData
    {
        uint32_t Width{};
        uint32_t Height{};
        std::string Title;
        GLFWwindow* Window{};
    };

    WindowData g_WindowData;

    void Window::Init(const uint32_t width, const uint32_t height, const char* title)
    {
        SPY_CORE_INFO("Creating window {0}", title);
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW!");
        }
        g_WindowData.Width = width;
        g_WindowData.Height = height;
        g_WindowData.Title = title;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        g_WindowData.Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!g_WindowData.Window)
        {
            throw std::runtime_error("Failed to create GLFW window!");
        }
        glfwMakeContextCurrent(g_WindowData.Window);
        glfwSetWindowUserPointer(g_WindowData.Window, &g_WindowData);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD!");
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glViewport(0, 0, width, height);

        glfwSetKeyCallback(g_WindowData.Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS) {
                Input::SetKeyStates(key, true);
            }else if (action == GLFW_RELEASE){
                Input::SetKeyStates(key, false);
            }
        });

        glfwSetMouseButtonCallback(g_WindowData.Window, [](GLFWwindow* window, int button, int action, int mods) {
            if (action == GLFW_PRESS) {
                Input::SetMouseButtonState(button, true);
            }else if (action == GLFW_RELEASE) {
                Input::SetMouseButtonState(button, false);
            }
        });

        glfwSetCursorPosCallback(g_WindowData.Window, [](GLFWwindow* window, double x, double y) {
            Input::SetMousePos(static_cast<float>(x),static_cast<float>(y));
        });
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::Close()
    {
        glfwSetWindowShouldClose(g_WindowData.Window, true);
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(g_WindowData.Window);
    }

    void Window::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    bool Window::IsOpen()
    {
        return !glfwWindowShouldClose(g_WindowData.Window);
    }

    void Window::SetVsync(bool value) {
        glfwSwapInterval(value);
    }

    uint32_t Window::GetWidth() const
    {
        return g_WindowData.Width;
    }

    uint32_t Window::GetHeight() const
    {
        return g_WindowData.Height;
    }
} // Spyen