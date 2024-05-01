#include "game/input/private/raylib_input_handlers.h"

#include "game/input/input.h"

namespace RaylibInputHandlers {
/// Input-related functions: keyboard
// bool IsKeyPressed(int key);        // Check if a key has been pressed once
// bool IsKeyPressedRepeat(int key);  // Check if a key has been pressed again (Only PLATFORM_DESKTOP)
// bool IsKeyDown(int key);           // Check if a key is being pressed
// bool IsKeyReleased(int key);       // Check if a key has been released once
// bool IsKeyUp(int key);             // Check if a key is NOT being pressed
// int GetKeyPressed(
//     void);  // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
// int GetCharPressed(
//     void);  // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
std::vector<Input::Info> Keyboard::ListInputs() const {
  return {
      Input::Info({
          .name = "Keyboard",
          .type = Input::Type::kKeyboard,
      }),
  };
}

Input::Samples Keyboard::Poll() { return {}; }

void Keyboard::SetConfig(int id, Input::Config) {}

/// Input-related functions: mouse
// bool IsMouseButtonPressed(int button);           // Check if a mouse button has been pressed once
// bool IsMouseButtonDown(int button);              // Check if a mouse button is being pressed
// bool IsMouseButtonReleased(int button);          // Check if a mouse button has been released once
// bool IsMouseButtonUp(int button);                // Check if a mouse button is NOT being pressed
// int GetMouseX(void);                             // Get mouse position X
// int GetMouseY(void);                             // Get mouse position Y
// Vector2 GetMousePosition(void);                  // Get mouse position XY
// Vector2 GetMouseDelta(void);                     // Get mouse delta between frames
// void SetMousePosition(int x, int y);             // Set mouse position XY
// void SetMouseOffset(int offsetX, int offsetY);   // Set mouse offset
// void SetMouseScale(float scaleX, float scaleY);  // Set mouse scaling
// float GetMouseWheelMove(void);                   // Get mouse wheel movement for X or Y, whichever is larger
// Vector2 GetMouseWheelMoveV(void);                // Get mouse wheel movement for both X and Y
// void SetMouseCursor(int cursor);                 // Set mouse cursor

std::vector<Input::Info> Mouse::ListInputs() const {
  return {
      Input::Info({
          .name = "Mouse",
          .type = Input::Type::kMouse,
      }),
  };
}

Input::Samples Mouse::Poll() { return {}; }

void Mouse::SetConfig(int id, Input::Config) {}

/// Input-related functions: touch
// int GetTouchX(void);                  // Get touch position X for touch point 0 (relative to screen size)
// int GetTouchY(void);                  // Get touch position Y for touch point 0 (relative to screen size)
// Vector2 GetTouchPosition(int index);  // Get touch position XY for a touch point index (relative to screen size)
// int GetTouchPointId(int index);       // Get touch point identifier for given index
// int GetTouchPointCount(void);         // Get number of touch points

std::vector<Input::Info> Touch::ListInputs() const {
  return {
      Input::Info({
          .name = "Touch",
          .type = Input::Type::kTouch,
      }),
  };
}

Input::Samples Touch::Poll() { return {}; }

void Touch::SetConfig(int id, Input::Config) {}

/// Input-related functions: gamepads
// bool IsGamepadAvailable(int gamepad);                   // Check if a gamepad is available
// const char *GetGamepadName(int gamepad);                // Get gamepad internal name id
// bool IsGamepadButtonPressed(int gamepad, int button);   // Check if a gamepad button has been pressed once
// bool IsGamepadButtonDown(int gamepad, int button);      // Check if a gamepad button is being pressed
// bool IsGamepadButtonReleased(int gamepad, int button);  // Check if a gamepad button has been released once
// bool IsGamepadButtonUp(int gamepad, int button);        // Check if a gamepad button is NOT being pressed
// int GetGamepadButtonPressed(void);                      // Get the last gamepad button pressed
// int GetGamepadAxisCount(int gamepad);                   // Get gamepad axis count for a gamepad
// float GetGamepadAxisMovement(int gamepad, int axis);    // Get axis movement value for a gamepad axis
// int SetGamepadMappings(const char *mappings);           // Set internal gamepad mappings (SDL_GameControllerDB)

std::vector<Input::Info> Gamepad::ListInputs() const { return {}; }

Input::Samples Gamepad::Poll() { return {}; }

void Gamepad::SetConfig(int id, Input::Config) {}
}  // namespace RaylibInputHandlers