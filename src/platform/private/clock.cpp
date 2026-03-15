// SPDX-FileCopyrightText: 2026 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#include "platform/clock.h"

#include <SDL3/SDL_timer.h>

Clock::time_point Clock::now() noexcept { return time_point{duration{SDL_GetTicksNS()}}; }
