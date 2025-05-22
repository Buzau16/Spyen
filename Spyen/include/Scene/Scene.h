#pragma once
#include "Core/Core.h"
#include <Core/weak_ptr.h>
#include <Scene/entt/entt.hpp>
#include "Core/LogMacros.h"
#include "Time/Timestep.h"

#include <functional>

namespace Spyen {

    using System =  std::function<void(Timestep)>;
    class Entity;

    class SPYEN_API Scene {
    public:
        weak_ptr<Entity> CreateEntity(const std::string& name);
        weak_ptr<Entity> GetEntityByName(const std::string& name);
        std::vector<weak_ptr<Entity>> GetEntitiesByTag(const std::string& tag);
        void AddSystem(const System& system);


        void RenderScene();
        void RunSceneSystems(Timestep ts) const;

    private:
        entt::registry m_Registry;
        std::unordered_map<std::string, std::shared_ptr<Entity>> m_Entities;
        std::vector<System> m_Systems;

        friend class Entity;
    };
}
