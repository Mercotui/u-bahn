// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

template <class... Ts>
struct OverloadedVisit : Ts... {
  using Ts::operator()...;
};
