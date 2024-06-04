#pragma once

#include <mp-units/chrono.h>
#include <mp-units/format.h>
#include <mp-units/systems/si/si.h>

namespace Units {
using Distance = mp_units::quantity<mp_units::si::metre>;

using TimeDelta = mp_units::quantity<mp_units::si::second>;

using Speed = mp_units::quantity<Distance::unit / TimeDelta::unit>;

using Acceleration = mp_units::quantity<Speed::unit / TimeDelta::unit>;
}  // namespace Units
