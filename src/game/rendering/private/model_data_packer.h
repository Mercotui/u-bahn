// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <fastgltf/core.hpp>

#include "game/rendering/model.h"

namespace ModelDataPacker {
ModelData PackBuffers(const fastgltf::Asset& asset);
}
