#include "InputHandler.h"

InputHandler::InputHandler()
    : keyboardState(nullptr)
    , mouseX(0)
    , mouseY(0)
    , mouseButtons(0)
{
}

void InputHandler::update() {
    keyboardState = SDL_GetKeyboardState(nullptr);
    mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
}

InputState InputHandler::getInputState(const Vector2& cameraPos) {
    InputState state;

    // Get movement direction
    state.moveDirection = getMovementVector();

    // Get aim position (mouse position in world coordinates)
    // For simplicity, using screen coordinates
    state.aimPosition = cameraPos + Vector2(mouseX - 640, mouseY - 360);

    // Shooting
    state.shoot = (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

    // Reload
    state.reload = keyboardState && keyboardState[SDL_SCANCODE_R];

    // Quit
    state.quit = keyboardState && keyboardState[SDL_SCANCODE_ESCAPE];

    return state;
}

Vector2 InputHandler::getMovementVector() {
    if (!keyboardState) return Vector2(0, 0);

    Vector2 movement(0, 0);

    if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP]) {
        movement.y -= 1.0f;
    }
    if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]) {
        movement.y += 1.0f;
    }
    if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) {
        movement.x -= 1.0f;
    }
    if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) {
        movement.x += 1.0f;
    }

    return movement;
}
