#include <unordered_map>
#include <Input/Input.h>

namespace Spyen {
    struct InputData {
        std::unordered_map<KeyCode, bool> Keys;
        std::unordered_map<KeyCode, bool> PrevKeys;
        std::unordered_map<MouseCode, bool> MouseBtns;
        std::unordered_map<MouseCode, bool> PrevMouseBtns;
        float MouseX{};
        float MouseY{};
    };

    static InputData s_Data;

    void Input::Update() {
        s_Data.PrevKeys = s_Data.Keys;
        s_Data.PrevMouseBtns = s_Data.MouseBtns;
    }

    void Input::SetKeyStates(KeyCode code, bool pressed) {
        s_Data.Keys[code] = pressed;
    }

    bool Input::IsKeyDown(KeyCode code) {
        return s_Data.Keys[code] && !s_Data.PrevKeys[code];
    }

    bool Input::IsKeyUp(KeyCode code)
    {
        return !s_Data.Keys[code] && s_Data.PrevKeys[code];
    }

    bool Input::IsKeyPressed(KeyCode code)
    {
        return s_Data.Keys[code];
    }

    void Input::SetMouseButtonState(MouseCode code, bool pressed)
    {
        s_Data.MouseBtns[code] = pressed;
    }

    void Input::SetMousePos(float x, float y) {
        s_Data.MouseX = x;
        s_Data.MouseY = y;
    }

    bool Input::IsMouseButtonDown(MouseCode code) {
        return s_Data.MouseBtns[code] && !s_Data.PrevMouseBtns[code];
    }

    bool Input::IsMouseButtonUp(MouseCode code) {
        return !s_Data.MouseBtns[code] && s_Data.PrevMouseBtns[code];
    }

    bool Input::IsMouseButtonPressed(MouseCode code)
    {
        return s_Data.MouseBtns[code];
    }

    float Input::GetMouseX()
    {
        return s_Data.MouseX;
    }

    float Input::GetMouseY()
    {
        return s_Data.MouseY;
    }


}
