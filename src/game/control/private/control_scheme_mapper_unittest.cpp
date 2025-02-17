#include "game/control/control_scheme_mapper.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <vector>

#include "game/input/keyboard_mouse.h"

namespace {
using KeyboardMouseInput::Key;

Input::Button& GetButton(const std::shared_ptr<Input>& input, Key key) {
  return input->buttons[static_cast<unsigned>(key)];
}
}  // namespace

TEST(ControlSchemeMapperTest, MapTrainControlsKeyboard) {
  ControlSchemeMapper mapper;

  auto input = std::make_shared<Input>(1, "keyboard", Input::Type::kKeyboard, true,
                                       std::vector<Input::Button>(static_cast<unsigned>(Key::kEnumKeySize)));

  auto& w = GetButton(input, Key::kW);
  auto& s = GetButton(input, Key::kS);
  auto& r = GetButton(input, Key::kR);

  // Nothing pressed
  const auto controls1 = mapper.MapGameControls({input});
  const auto train_controls1 = controls1.train_controls;
  ASSERT_EQ(train_controls1.throttle, 0.0f);
  ASSERT_EQ(train_controls1.brake, 0.0f);

  // Key W down
  w.down = true;
  const auto controls2 = mapper.MapGameControls({input});
  const auto train_controls2 = controls2.train_controls;
  ASSERT_EQ(train_controls2.throttle, 1.0f);
  ASSERT_EQ(train_controls2.brake, 0.0f);

  // Key W lifts, key S down
  w.down = false;
  s.down = true;
  const auto controls3 = mapper.MapGameControls({input});
  const auto train_controls3 = controls3.train_controls;
  ASSERT_EQ(train_controls3.throttle, 0.0f);
  ASSERT_EQ(train_controls3.brake, 1.0f);

  // key S lifts, key R down
  s.down = false;
  r.down = true;
  r.changed = true;
  const auto controls4 = mapper.MapGameControls({input});
  const auto train_controls4 = controls4.train_controls;
  ASSERT_EQ(train_controls4.throttle, 0.0f);
  ASSERT_EQ(train_controls4.brake, 0.0f);
  ASSERT_TRUE(train_controls4.reverse);

  // R still down but not changed
  r.changed = false;
  const auto controls5 = mapper.MapGameControls({input});
  const auto train_controls5 = controls5.train_controls;
  ASSERT_EQ(train_controls5.throttle, 0.0f);
  ASSERT_EQ(train_controls5.brake, 0.0f);
  ASSERT_FALSE(train_controls5.reverse);

  // Nothing down
  r.changed = false;
  const auto controls6 = mapper.MapGameControls({input});
  const auto train_controls6 = controls6.train_controls;
  ASSERT_EQ(train_controls6.throttle, 0.0f);
  ASSERT_EQ(train_controls6.brake, 0.0f);
  ASSERT_FALSE(train_controls6.reverse);
}

TEST(ControlSchemeMapperTest, MapTrainControlsTouch) {
  ControlSchemeMapper mapper;

  auto input = std::make_shared<Input>(1, "touch", Input::Type::kTouch, true);
  input->buttons = {Input::Button{"Touch 1", true, true}};
  input->axes = {Input::Axis{.name = "Touch Axis Horizontal", .value = 43.0f, .active = false},
                 Input::Axis{.name = "Touch Axis Vertical", .value = 1000.0f, .active = false}};

  // Initial touch event
  const auto controls1 = mapper.MapGameControls({input});
  const auto train_controls1 = controls1.train_controls;
  ASSERT_EQ(train_controls1.throttle, 0.0f);
  ASSERT_EQ(train_controls1.brake, 0.0f);

  // Finger moves up towards top of screen, which has lower X coordinate
  input->axes[1].value = 500.0f;
  input->buttons[0].changed = false;
  const auto controls2 = mapper.MapGameControls({input});
  const auto train_controls2 = controls2.train_controls;
  ASSERT_EQ(train_controls2.throttle, 5.0f);
  ASSERT_EQ(train_controls2.brake, 0.0f);

  // Finger moves down towards bottom of screen, which has higher X coordinate
  input->axes[1].value = 1500.0f;
  input->buttons[0].changed = false;
  const auto controls3 = mapper.MapGameControls({input});
  const auto train_controls3 = controls3.train_controls;
  ASSERT_EQ(train_controls3.throttle, 0.0f);
  ASSERT_EQ(train_controls3.brake, 5.0f);

  // Finger back at starting position
  input->axes[1].value = 1000.0f;
  const auto controls4 = mapper.MapGameControls({input});
  const auto train_controls4 = controls4.train_controls;
  ASSERT_EQ(train_controls4.throttle, 0.0f);
  ASSERT_EQ(train_controls4.throttle, 0.0f);

  // Touch lift event, finger no longer touching screen
  input->axes[1].value = 1000.0f;
  input->buttons[0].down = false;
  input->buttons[0].changed = true;
  const auto controls5 = mapper.MapGameControls({input});
  const auto train_controls5 = controls5.train_controls;
  ASSERT_EQ(train_controls5.throttle, 0.0f);
  ASSERT_EQ(train_controls5.throttle, 0.0f);
}

TEST(ControlSchemeMapperTest, MapTrainControlsActiveSwitching) {
  ControlSchemeMapper mapper;

  const auto touch = std::make_shared<Input>(1, "touch", Input::Type::kTouch, false);
  touch->buttons = {Input::Button{"Touch 1", false, false}};
  touch->axes = {Input::Axis{.name = "Touch Axis Horizontal", .value = 43.0f, .active = false},
                 Input::Axis{.name = "Touch Axis Vertical", .value = 1000.0f, .active = false}};
  const auto keyboard = std::make_shared<Input>(2, "keyboard", Input::Type::kKeyboard, false,
                                                std::vector<Input::Button>(static_cast<unsigned>(Key::kEnumKeySize)));

  InputList input_list = {touch, keyboard};

  auto do_touch_event = [&mapper, &input_list](const std::shared_ptr<Input>& input) {
    // Finger makes contact
    input->active = true;
    input->buttons[0].changed = true;
    input->buttons[0].down = true;
    input->axes[1].value = 1000.0f;
    const auto controls1 = mapper.MapGameControls({input});
    const auto train_controls1 = controls1.train_controls;
    ASSERT_EQ(train_controls1.throttle, 0.0f);

    // Finger moves up towards top of screen, which has lower X coordinate
    input->axes[1].value = 500.0f;
    input->buttons[0].changed = false;
    const auto controls2 = mapper.MapGameControls({input});
    const auto train_controls2 = controls2.train_controls;
    ASSERT_EQ(train_controls2.throttle, 5.0f);

    // Reset
    input->active = false;
    input->buttons[0].changed = false;
    input->buttons[0].down = false;
  };

  auto do_keyboard_event = [&mapper, &input_list](const std::shared_ptr<Input>& input) {
    auto& w = GetButton(input, Key::kW);
    // Nothing pressed
    const auto controls1 = mapper.MapGameControls({input});
    const auto train_controls1 = controls1.train_controls;
    ASSERT_EQ(train_controls1.throttle, 0.0f);

    // Key W down
    input->active = true;
    w.down = true;
    w.changed = true;
    const auto controls2 = mapper.MapGameControls({input});
    const auto train_controls2 = controls2.train_controls;
    ASSERT_EQ(train_controls2.throttle, 1.0f);
    ASSERT_EQ(train_controls2.brake, 0.0f);

    // Reset
    input->active = false;
    w.down = false;
    w.changed = false;
  };

  do {
    for (const auto& input : input_list) {
      switch (input->type) {
        case Input::Type::kTouch: {
          do_touch_event(input);
          break;
        }
        case Input::Type::kKeyboard: {
          do_keyboard_event(input);
          break;
        }
        case Input::Type::kMouse: {
          FAIL() << "Not implemented";
        }
        case Input::Type::kJoystick: {
          FAIL() << "Not implemented";
        }
      }
    }
  } while (std::ranges::next_permutation(input_list.begin(), input_list.end()).found);
}
