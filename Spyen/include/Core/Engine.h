#pragma once
#include "Core/Core.h"
#include <cstdint>

namespace Spyen {

class SPYEN_API Engine {
public:
    static void Init(uint32_t width, uint32_t height, const char* title);
    static void Run();
};

} // Spyen
