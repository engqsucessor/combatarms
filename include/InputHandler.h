#pragma once

#include "Vector2.h"
#include <SDL2/SDL.h>

struct InputState {
    Vector2 moveDirection;
    Vector2 aimPosition;
    bool shoot;
    bool reload;
    bool quit;

    InputState() : moveDirection(0, 0), aimPosition(0, 0), shoot(false), reload(false), quit(false) {}
};

class InputHandler {
private:
    const Uint8* keyboardState;
    int mouseX, mouseY;
    Uint32 mouseButtons;

public:
    InputHandler();

    void update();
    InputState getInputState(const Vector2& cameraPos);

private:
    Vector2 getMovementVector();
};
