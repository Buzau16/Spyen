#include <Spyen.h>
#define ARCBYTE_IMPLEMENTATION
#include "Reader.h"
#include "spdlog/common.h"

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
    FileContext* context = reader_init("coxx.aby");
    ABHashEntry entry = get_entry(context, "Player");
    auto texture = Spyen::Texture::Create(
            static_cast<const unsigned char *>(reader_get_entry(context, "Player")), entry.width, entry.height, entry.channels);
    ent->GetComponent<Spyen::RenderComponent>().Texture = texture;

    scene->AddSystem(Move);

    Spyen::Engine::SetActiveScene("1");

    Spyen::Engine::Run();

    reader_shutdown(context);
}