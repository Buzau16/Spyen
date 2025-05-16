#pragma once
#include "Core/Core.h"
#include <Math/glm.hpp>
#include "Math/ext/matrix_transform.hpp"

namespace Spyen {
    struct TransformComponent {
        Vec2 Position = {0.0f, 0.0f};
        float Rotation = 0.0f;
        float Scale = 1.0f;
        TransformComponent() = default;

        [[nodiscard]] glm::mat4 GetTransform() const {
            return glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, 1.0f)) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0, 0, 1)) *
                        glm::scale(glm::mat4(1.0f), glm::vec3(Scale, Scale, 1.0f));
        }
    };

    struct ColorComponent {
        Spyen::Color Color = {0.0f, 0.0f, 0.0f, 1.0f};
        ColorComponent() = default;
        ColorComponent(const Spyen::Color color) : Color(color) {};
    };
}
