// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

// This header depends on the platform compile definitions,
// please do not include it without also linking the platform lib.
#if defined(PLATFORM_WEB)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
