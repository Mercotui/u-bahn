#pragma once

#include <mp-units/chrono.h>
#include <mp-units/format.h>
#include <mp-units/systems/si/si.h>

namespace Units {
using TimeDelta = mp_units::quantity<mp_units::si::second>;

using Acceleration = mp_units::quantity<mp_units::si::metre / (mp_units::si::second * mp_units::si::second)>;

using Speed = mp_units::quantity<mp_units::si::metre / mp_units::si::second>;

using Distance = mp_units::quantity<mp_units::si::metre>;
}  // namespace Units
