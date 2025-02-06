#pragma once

#include <memory>
#include <vector>

#include "game/input/input_manager_interface.h"
#include "game/input/private/input_axis_helpers.h"

namespace RaylibInputHandlers {
class Keyboard final : public InputManagerInterface {
 public:
  /**
   * Constructor
   */
  Keyboard();

  /**
   * Poll the keyboard
   * @return the updated state of the keyboard
   */
  InputList Poll() override;

  /**
   * Does nothing
   */
  void SetConfig(int, Input::Config) override {};

 private:
  //! the state of the keyboard
  std::shared_ptr<Input> input_;
};

class Mouse final : public InputManagerInterface {
 public:
  /**
   * Constructor
   */
  Mouse();

  /**
   * Poll the mouse
   * @return the updated state of the mouse
   */
  InputList Poll() override;

  /**
   * Does nothing
   */
  void SetConfig(int, Input::Config) override {};

 private:
  //! the state of the mouse
  std::shared_ptr<Input> input_;
  //! activity detectors for each axis
  std::vector<InputAxisHelpers::ActivityDetector> axis_activity_;
};

class Touch final : public InputManagerInterface {
 public:
  /**
   * Constructor
   */
  Touch();

  /**
   * Poll the touchscreen
   * @return the updated state of the touchscreen
   */
  InputList Poll() override;

  /**
   * Does nothing
   */
  void SetConfig(int, Input::Config) override {};

 private:
  //! the state of the touchscreen
  std::shared_ptr<Input> input_;
};
}  // namespace RaylibInputHandlers
