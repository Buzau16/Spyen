#pragma once
#include <cstdint>

namespace Spyen {

class Window {
public:
    void Init(uint32_t width, uint32_t height, const char* title);
    void PollEvents();
    void Close();
    void SwapBuffers();
    void Clear(float r, float g, float b, float a = 1.0f);
    bool IsOpen();
    void SetVsync(bool value);

    [[nodiscard]] inline uint32_t GetWidth() const;
    [[nodiscard]] inline uint32_t GetHeight() const;
};

} // Spyen