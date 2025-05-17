#include <Spyen.h>

void Move(Spyen::Timestep ts) {
    Spyen::Engine::GetSceneByName("1")->GetEntityByName("test")->GetComponent<Spyen::TransformComponent>().Rotation += 10.0f * ts;
    Spyen::Engine::GetSceneByName("1")->GetEntityByName("test")->GetComponent<Spyen::ColorComponent>().Color.r -= 0.1f * ts;
}

int main(int argc, char* argv[])
{

    Spyen::Engine::Init(800,600,"Spyen");
    Spyen::Engine::SetBackgroundColor(0.2f, 0.3f, 0.8f);
    Spyen::Engine::SetVsync(false);

    auto scene = Spyen::Engine::CreateScene("1");
    auto ent = scene->CreateEntity("test");
    ent->AddComponent<Spyen::TransformComponent>();
    ent->AddComponent<Spyen::ColorComponent>();
    ent->GetComponent<Spyen::ColorComponent>().Color = {1.0f, 1.0f, 1.0f};
    ent->GetComponent<Spyen::TransformComponent>().Rotation = 45.0f;

    scene->AddSystem(Move);

    Spyen::Engine::SetActiveScene("1");

    Spyen::Engine::Run();
}
