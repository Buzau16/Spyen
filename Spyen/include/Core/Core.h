#pragma once

#ifdef SPYEN_BUILD_DLL
#define SPYEN_API __attribute__((visibility("default")))
#else
#define SPYEN_API
#endif


namespace Spyen {
    struct Color
    {
        float r, g, b, a = 1.0f;
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
