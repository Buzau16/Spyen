#include <Core/Engine.h>

#include "Scene/Components.h"
#include "Scene/Scene.h"

int main(int argc, char* argv[])
{

    Spyen::Engine::Init(800,600,"Spyen");
    Spyen::Engine::SetBackgroundColor(0.2f, 0.3f, 0.8f);

    auto scene = std::make_shared<Spyen::Scene>();
    scene->CreateEntity("name");
    scene->AddComponent<Spyen::TransformComponent>("name");
    scene->AddComponent<Spyen::ColorComponent>("name");
    scene->GetComponent<Spyen::ColorComponent>("name").Color = {1.0f, 1.0f, 1.0f, 1.0f};
    scene->GetComponent<Spyen::TransformComponent>("name").Rotation = 45.0f;


    Spyen::Engine::SetActiveScene(scene);

    Spyen::Engine::Run();
}
