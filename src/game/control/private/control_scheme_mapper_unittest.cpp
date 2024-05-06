#include "game/control/control_scheme_mapper.h"

#include <gtest/gtest.h>

namespace {
using Control::Mode;
}

TEST(ControlSchemeMapperTest, MapTrainControls) {
  const InputList inputs;
  ControlSchemeMapper mapper;
  const auto controls = mapper.Map(inputs, Mode::kTrain);
}
