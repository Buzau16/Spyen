#include <string>
#include <Window/Window.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "Core/LogMacros.h"

namespace Spyen {
    struct WindowData
    {
        uint32_t Width{};
        uint32_t Height{};
        std::string Title;
        GLFWwindow* Window{};
    };

    static WindowData s_WindowData;

    void Window::Init(const uint32_t width, const uint32_t height, const char* title)
    {
        SPY_CORE_INFO("Creating window {0}", title);
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW!");
        }
        s_WindowData.Width = width;
        s_WindowData.Height = height;
        s_WindowData.Title = title;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        s_WindowData.Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!s_WindowData.Window)
        {
            throw std::runtime_error("Failed to create GLFW window!");
        }
        glfwMakeContextCurrent(s_WindowData.Window);
        glfwSetWindowUserPointer(s_WindowData.Window, &s_WindowData);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD!");
        }



    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::Close()
    {
        glfwSetWindowShouldClose(s_WindowData.Window, true);
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(s_WindowData.Window);
    }

    void Window::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    bool Window::IsOpen()
    {
        return !glfwWindowShouldClose(s_WindowData.Window);
    }

    uint32_t Window::GetWidth() const
    {
        return s_WindowData.Width;
    }

    uint32_t Window::GetHeight() const
    {
        return s_WindowData.Height;
    }
} // Spyen