#pragma once

#include <Core/Core.h>

#include "Core/Keycodes.h"
#include "Core/Mousecodes.h"

namespace Spyen {
    class SPYEN_API Input {
    public:
        static void Update();

        static bool IsKeyDown(KeyCode code);
        static bool IsKeyUp(KeyCode code);
        static bool IsKeyPressed(KeyCode code);

        static bool IsMouseButtonDown(MouseCode code);
        static bool IsMouseButtonUp(MouseCode code);
        static bool IsMouseButtonPressed(MouseCode code);
        static float GetMouseX();
        static float GetMouseY();

    private:
        friend class Window;
        static void SetKeyStates(KeyCode code, bool pressed);
        static void SetMouseButtonState(MouseCode code, bool pressed);
        static void SetMousePos(float x, float y);
    };
}
