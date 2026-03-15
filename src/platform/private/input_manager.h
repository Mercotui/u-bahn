// SPDX-FileCopyrightText: 2026 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <SDL3/SDL_events.h>

#include "platform/input.h"

class InputManager {
 public:
  const Inputs& GetInputs();

  /**
   * Update the input state based on an SDL event
   * @param event the event to handle, only input events will be handled, other event types are ignored.
   * @return true if the event was handled, false if the event was ignored
   */
  bool HandleEvent(const SDL_Event& event);

  /// Clear changed-flags after the inputs have been handled
  void ClearChanges();

 private:
  Inputs inputs_;
};
