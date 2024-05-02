#include "game/input/private/joystick_handler.h"

#include <gtest/gtest.h>

#include "game/input/private/input_unittest_helpers.h"

namespace {
using InputUnittestHelpers::CountInputName;
using InputUnittestHelpers::CountInputType;

constexpr auto kPrerequisiteJoystickName = "euh, I should go find the joystick name";
}  // namespace

/**
 * Test fixture to test JoystickHandler.
 *
 * These tests require a specific joystick to be attached,
 * if this joystick is not found then these tests are skipped.
 */
class JoystickHandlerTestFixture : public ::testing::Test {
 protected:
  void SetUp() override {
    const auto list = handler.Poll();
    if (!CountInputName(list, kPrerequisiteJoystickName)) {
      GTEST_SKIP() << "Skipping all tests for this fixture, joystick with name=\"" << kPrerequisiteJoystickName
                   << "\" was not found";
    }
  }

  JoystickHandler handler;
};

TEST_F(JoystickHandlerTestFixture, Poll) {
  auto list = handler.Poll();
  EXPECT_EQ(list.size(), 2);
};

TEST_F(JoystickHandlerTestFixture, SetConfig) {
  const auto list_before = handler.Poll();
  EXPECT_EQ(list_before.size(), 2);

  const int id_to_disable = list_before.front()->id;
  Input::Config config;
  config.enabled = false;
  handler.SetConfig(id_to_disable, config);

  const auto list_after = handler.Poll();
  EXPECT_EQ(list_after.size(), 1);
};