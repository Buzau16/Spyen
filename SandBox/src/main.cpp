#include <iostream>
#include <Spyen.h>

void Move(Spyen::Timestep ts) {
    auto& trans = Spyen::Engine::GetSceneByName("1")->GetEntityByName("test")->GetComponent<Spyen::TransformComponent>();
    if (Spyen::Input::IsKeyPressed(Spyen::SPK_W)) {
        trans.Position.y += 1.f * ts;
    }
    if (Spyen::Input::IsKeyPressed(Spyen::SPK_S)) {
        trans.Position.y -= 1.f * ts;
    }
    if (Spyen::Input::IsKeyPressed(Spyen::SPK_D)) {
        trans.Position.x += 1.f * ts;
    }
    if (Spyen::Input::IsKeyPressed(Spyen::SPK_A)) {
        trans.Position.x -= 1.f * ts;
    }
}

int main(int argc, char* argv[])
{
    Spyen::Engine::Init(640,360,"Spyen");
    Spyen::Engine::SetBackgroundColor(0.2f, 0.3f, 0.8f);
    Spyen::Engine::SetVsync(false);

    auto scene = Spyen::Engine::CreateScene("1");
    auto ent = scene->CreateEntity("test");
    auto& renderComp = ent->GetComponent<Spyen::RenderComponent>();
renderComp.Color = {1.0f, 1.0f, 1.0f, 1.0f};
std::cout << "Color set: " << renderComp.Color.r << ", " << renderComp.Color.g << ", " 
          << renderComp.Color.b << ", " << renderComp.Color.a << std::endl;
    ent->GetComponent<Spyen::TransformComponent>().Rotation = 0.0f;

    scene->AddSystem(Move);

    Spyen::Engine::SetActiveScene("1");

    Spyen::Engine::Run();
}