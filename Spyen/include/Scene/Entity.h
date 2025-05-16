#pragma once
#include <Core/Core.h>

#include "entt/entity/entity.hpp"

namespace Spyen {
    class Scene;

    class SPYEN_API Entity {
    public:
        Entity() = default;
        Entity(entt::entity entity);

    private:
        entt::entity m_Entity = entt::null;
        friend class Scene;
    };
}