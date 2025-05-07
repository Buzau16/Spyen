#include <iostream>
#include <Core/Engine.h>

#include "Core/LogMacros.h"

int main(int argc, char* argv[])
{

    Spyen::Engine::Init(800,600,"Hi");
    SPY_INFO("Hello");

    Spyen::Engine::Run();
}
