#include <Scene/Scene.h>

#include "Renderer/Renderer.h"
#include "Scene/Components.h"

namespace Spyen {
    void Scene::CreateEntity(const std::string &name) {
        Entity entity = Entity{m_Registry.create()};
        SPY_CORE_INFO("Creating entity {0}", name);
        m_Entities[name] = entity;
    }

    Entity Scene::GetEntityByName(const std::string &name) {
        if (m_Entities.contains(name)) {
            return m_Entities[name];
        }else {
            SPY_CORE_ERROR("Entity with name {0} not found", name);
            return Entity{};
        }
    }

    void Scene::RenderScene() {
        auto view = m_Registry.view<ColorComponent, TransformComponent>();

        Renderer::BeginBatch();
        for (auto [entity, color, transform] : view.each()) {
            Renderer::SubmitQuad(transform.GetTransform(), color.Color);
        }

        Renderer::EndBatch();
    }
}
