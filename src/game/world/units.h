// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <mp-units/format.h>
#include <mp-units/systems/isq_angle.h>
#include <mp-units/systems/si.h>

namespace Units {
using mp_units::quantity;

using Distance = quantity<mp_units::si::metre>;

using TimeDelta = quantity<mp_units::si::second>;

using Speed = quantity<Distance::unit / TimeDelta::unit>;

using Acceleration = quantity<Speed::unit / TimeDelta::unit>;

using Angle = quantity<mp_units::si::radian>;

using AngularSpeed = quantity<Angle::unit / TimeDelta::unit>;

using AngularAcceleration = quantity<AngularSpeed::unit / TimeDelta::unit>;
}  // namespace Units
