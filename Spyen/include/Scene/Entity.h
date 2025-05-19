#pragma once
#include <Core/Core.h>

#include "entt/entity/entity.hpp"

#include "Scene.h"

namespace Spyen {
    class SPYEN_API Entity {
    public:
        Entity() = default;
        Entity(entt::entity entity, Scene* scene);

        template<typename T, typename... Args>
        void AddComponent(Args&&... args) {

            if (m_ParentScene->m_Registry.any_of<T>(m_EntityHandle)) {
                SPY_CORE_ERROR("Entity already has this component!");
                return;
            }

            m_ParentScene->m_Registry.emplace<T>(m_EntityHandle,  std::forward<Args>(args)...);
        }

        template<typename T>
        void RemoveComponent() const {
            m_ParentScene->m_Registry.remove<T>(m_EntityHandle);
        }

        template<typename T>
        [[nodiscard]] bool HasComponent() const {
            return m_ParentScene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        T& GetComponent() {
            if (!HasComponent<T>()) {
                SPY_CORE_ERROR("Entity does not have this component!");
                throw std::runtime_error("Component does not exist");
            }
            return m_ParentScene->m_Registry.get<T>(m_EntityHandle);
        }

        bool operator==(const Entity& other) const
        {
            return m_EntityHandle == other.m_EntityHandle && m_ParentScene == other.m_ParentScene;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

    private:
        Scene* m_ParentScene{};
        entt::entity m_EntityHandle = entt::null;
        friend class Scene;
    };
}