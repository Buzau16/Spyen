#pragma once
#include "Core/Core.h"
#include <cstdint>
#include <memory>

#include "Scene/Scene.h"

namespace Spyen {

class SPYEN_API Engine {
public:
    static void Init(uint32_t width, uint32_t height, const char* title);
    static void SetBackgroundColor(float r, float g, float b, float a = 1.0f);
    static void SetActiveScene(const std::shared_ptr<Scene> &scene);
    static void Run();
};

} // Spyen
