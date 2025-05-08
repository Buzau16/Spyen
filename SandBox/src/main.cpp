#include <Core/Engine.h>

int main(int argc, char* argv[])
{

    Spyen::Engine::Init(800,600,"Spyen");
    Spyen::Engine::SetBackgroundColor(0.2f, 0.3f, 0.8f);

    Spyen::Engine::Run();
}
