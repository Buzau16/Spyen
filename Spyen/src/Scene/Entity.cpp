#include <Scene/Entity.h>
#include <Scene/Scene.h>


namespace Spyen
{
    Entity::Entity(entt::entity entity, Scene* scene) : m_EntityHandle(entity), m_ParentScene(scene){
    }
}
