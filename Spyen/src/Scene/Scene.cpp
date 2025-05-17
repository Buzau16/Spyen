#include <Scene/Entity.h>
#include <Scene/Scene.h>
#include "Renderer/Renderer.h"
#include "Scene/Components.h"

namespace Spyen {
    weak_ptr<Entity> Scene::CreateEntity(const std::string &name) {
        if (!m_Entities.contains(name)) {
            SPY_CORE_INFO("Creating entity {0}", name);
            m_Entities[name] = std::make_shared<Entity>(m_Registry.create(), this);
            return weak_ptr<Entity>(m_Entities.at(name));
        }else {
            SPY_CORE_ERROR("Entity: {} already exists!", name);
        }
    }

    weak_ptr<Entity> Scene::GetEntityByName(const std::string &name) {
        if (m_Entities.contains(name)) {
            return weak_ptr<Entity>(m_Entities.at(name));
        }else {
            SPY_CORE_ERROR("Entity with name {0} not found", name);
            throw std::runtime_error("Entity not found");
        }
    }

    void Scene::AddSystem(const System& system) {
        m_Systems.push_back(system);
    }

    void Scene::RenderScene() {
        const auto view = m_Registry.view<ColorComponent, TransformComponent>();

        Renderer::BeginBatch();
        for (auto [entity, color, transform] : view.each()) {
            Renderer::SubmitQuad(transform.GetTransform(), color.Color);
        }

        Renderer::EndBatch();
    }

    void Scene::RunSceneSystems(const Timestep ts) const {
        for (auto& system : m_Systems) {
            system(ts);
        }
    }
}
