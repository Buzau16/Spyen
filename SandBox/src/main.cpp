#include <Core/Engine.h>

#include "Scene/Components.h"
#include "Scene/Scene.h"

int main(int argc, char* argv[])
{

    Spyen::Engine::Init(800,600,"Spyen");
    Spyen::Engine::SetBackgroundColor(0.2f, 0.3f, 0.8f);

    auto scene = Spyen::Engine::CreateScene("1");
    scene->CreateEntity("test");
    scene->AddComponent<Spyen::TransformComponent>("test");
    scene->AddComponent<Spyen::ColorComponent>("test");
    scene->GetComponent<Spyen::ColorComponent>("test").Color = {1.0f, 1.0f, 1.0f};
    scene->GetComponent<Spyen::TransformComponent>("test").Rotation = 45.0f;

    Spyen::Engine::SetActiveScene("1");

    Spyen::Engine::Run();
}
