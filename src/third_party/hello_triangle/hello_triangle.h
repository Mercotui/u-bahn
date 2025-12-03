// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <glm/mat4x4.hpp>

namespace HelloTriangle {
int Init();
void Shutdown();

void Draw(const glm::mat4& transform);
}  // namespace HelloTriangle
