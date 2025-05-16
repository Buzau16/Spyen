#pragma once
#include "Core/Core.h"
#include <Scene/Entity.h>

#include <Scene/entt/entt.hpp>

#include "Core/LogMacros.h"

namespace Spyen {
    class SPYEN_API Scene {
    public:
        void CreateEntity(const std::string& name);
        Entity GetEntityByName(const std::string& name);

        template<typename T, typename... Args>
        void AddComponent(const std::string& name, Args&&... args) {
            if (!ContainsEntity(name)) {
                SPY_CORE_CRITICAL("Entity {} does not exist!", name);
                return;
            }

            if (m_Registry.any_of<T>(m_Entities[name].m_Entity)) {
                SPY_CORE_ERROR("Entity {} already has this component!. {}", name, __LINE__);
                return;
            }

            m_Registry.emplace<T>(m_Entities[name].m_Entity,  std::forward<Args>(args)...);
        }

        template<typename T>
        void RemoveComponent(const std::string& name) {
            if (!ContainsEntity(name)) {
                SPY_CORE_CRITICAL("Entity {} does not exist!", name);
                return;
            }

            m_Registry.remove<T>(name);
        }

        template<typename T>
        bool HasComponent(const std::string& name) {
            if (!ContainsEntity(name)) {
                SPY_CORE_CRITICAL("Entity {} does not exist!", name);
                return false;
            }

            return m_Registry.all_of<T>(name);
        }

        template<typename T>
        T& GetComponent(const std::string& name) {
            if (!ContainsEntity(name)) {
                SPY_CORE_CRITICAL("Entity {} does not exist!", name);
                throw std::runtime_error("Entity does not exist");
            }

            if (!m_Registry.any_of<T>(m_Entities[name].m_Entity)) {
                SPY_CORE_ERROR("Entity {} does not have this component!", name);
                throw std::runtime_error("Component does not exist");
            }

            return m_Registry.get<T>(m_Entities[name].m_Entity);
        }

        void RenderScene();

    private:
        inline bool ContainsEntity(const std::string& index) const {
            return m_Entities.contains(index);
        }

        entt::registry m_Registry;
        std::unordered_map<std::string, Entity> m_Entities;

        friend class Entity;
    };
}
