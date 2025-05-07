#pragma once
#include <cstdint>

namespace Spyen {

class Engine {
public:
    static void Init(uint32_t width, uint32_t height, const char* title);
    static void Run();
};

} // Spyen
