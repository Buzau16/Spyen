#pragma once

#ifdef SPYEN_BUILD_DLL
#define SPYEN_API __declspec(dllexport)
#else
#define SPYEN_API __declspec(dllimport)
#endif
#include <cstdint>


namespace Spyen {
    struct Color
    {
        float r, g, b, a;
    };

    struct Vec2
    {
        float x, y;
    };

    struct Vec3
    {
        float x, y, z;
    };

    struct Vec4
    {
        float x, y, z, w;
    };

}
