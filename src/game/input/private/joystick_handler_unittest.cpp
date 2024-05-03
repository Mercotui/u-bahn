#include "game/input/private/joystick_handler.h"

#include <gtest/gtest.h>

#include "game/input/private/input_unittest_helpers.h"

namespace {
using InputUnittestHelpers::CountInputName;
using InputUnittestHelpers::CountInputType;

constexpr auto kPrerequisiteJoystickName = "Mad Catz Mad Catz F.L.Y.5 Stick";
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
  const auto list = handler.Poll();
  EXPECT_EQ(list.size(), 2);

  auto joy_it = std::find_if(std::begin(list), std::end(list),
                             [](const auto& input) { return input->name == kPrerequisiteJoystickName; });
  EXPECT_NE(joy_it, std::end(list));

  const auto& input = *joy_it;
  EXPECT_TRUE(input->config.enabled);
  EXPECT_EQ(input->buttons.size(), 14);
  EXPECT_EQ(input->buttons.front().name, "Button 1");
  EXPECT_EQ(input->buttons.back().name, "Button 14");
  EXPECT_EQ(input->axes.size(), 7);
  EXPECT_EQ(input->axes.front().name, "Axis 1");
  EXPECT_EQ(input->axes.back().name, "Axis 7");
}

TEST_F(JoystickHandlerTestFixture, SetConfig) {
  const auto list_before = handler.Poll();
  EXPECT_EQ(list_before.size(), 2);

  const int id_to_disable = list_before.front()->id;
  Input::Config config;

  config.enabled = false;
  handler.SetConfig(id_to_disable, config);
  const auto list_after1 = handler.Poll();
  EXPECT_EQ(list_after1.size(), 1);

  config.enabled = true;
  handler.SetConfig(id_to_disable, config);
  const auto list_after2 = handler.Poll();
  EXPECT_EQ(list_after2.size(), 2);
}
