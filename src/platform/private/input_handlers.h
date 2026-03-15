// SPDX-FileCopyrightText: 2026 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <SDL3/SDL_events.h>

#include "platform/input.h"

namespace KeyboardHandler {
void Add(Inputs* inputs, const SDL_KeyboardDeviceEvent& event);
void Remove(Inputs* inputs, const SDL_KeyboardDeviceEvent& event);

void SetKey(Inputs* inputs, const SDL_KeyboardEvent& event);
}  // namespace KeyboardHandler

namespace MouseHandler {
void Add(Inputs* inputs, const SDL_MouseDeviceEvent& event);
void Remove(Inputs* inputs, const SDL_MouseDeviceEvent& event);

void SetPosition(Inputs* inputs, const SDL_MouseMotionEvent& event);
void SetWheel(Inputs* inputs, const SDL_MouseWheelEvent& event);

void SetButton(Inputs* inputs, const SDL_MouseButtonEvent& event);
}  // namespace MouseHandler

namespace GamepadHandler {}
namespace TouchHandler {}
