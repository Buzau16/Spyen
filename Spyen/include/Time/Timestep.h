#pragma once

#include <Core/Core.h>

namespace Spyen {
    class SPYEN_API Timestep
    {
    public:
        explicit Timestep(const float time) : m_DeltaTime(time) {}

        ~Timestep() = default;

        operator float() const { return m_DeltaTime; };


    private:
        float m_DeltaTime;
    };
}