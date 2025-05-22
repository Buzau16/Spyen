#include <Scene/Entity.h>
#include <Scene/Scene.h>
#include "Renderer/Renderer.h"
#include "Scene/Components.h"

namespace Spyen {
    weak_ptr<Entity> Scene::CreateEntity(const std::string &name) {
        if (!m_Entities.contains(name)) {
            SPY_CORE_INFO("Creating entity {0}", name);
            m_Entities[name] = std::make_shared<Entity>(m_Registry.create(), this);
            auto& entity = m_Entities.at(name);
            entity->AddComponent<TransformComponent>();
            entity->AddComponent<RenderComponent>();
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

    std::vector<weak_ptr<Entity>> Scene::GetEntitiesByTag(const std::string &tag) {
        std::vector<weak_ptr<Entity>> entities;
        for (const auto& [name, tagged_entity] : m_Entities){
            if (tagged_entity->HasComponent<TagComponent>() &&
                tagged_entity->GetComponent<TagComponent>().Tag == tag){
                    entities.emplace_back(tagged_entity);
            }
        }

        return entities;
    }

    void Scene::AddSystem(const System& system) {
        m_Systems.push_back(system);
    }

    void Scene::RenderScene() {
        const auto view = m_Registry.view<RenderComponent, TransformComponent>();

        Renderer::BeginBatch();
        for (auto [entity, target, transform] : view.each()) {
            if (target.Texture == nullptr) {
                Renderer::SubmitQuad(transform.GetTransform(), target.Color);
                SPY_CORE_INFO("Rendering Colored entity");
            }else {
                Renderer::SubmitQuad(transform.GetTransform(), target.Texture);
                //SPY_CORE_INFO("Rendering textured entity");
            }

        }

        Renderer::EndBatch();
    }

    void Scene::RunSceneSystems(const Timestep ts) const {
        for (auto& system : m_Systems) {
            system(ts);
        }
    }
}
