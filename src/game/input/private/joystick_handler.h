#pragma once

#include <map>
#include <memory>
#include <vector>

#include "game/input/input.h"
#include "game/input/input_manager_interface.h"
#include "libenjoy/libenjoy.h"

class JoystickHandler final : public InputManagerInterface {
 public:
  /**
   * Constructor, scans and opens all joysticks.
   */
  JoystickHandler();

  /**
   * Destructor, closes all opened joysticks.
   */
  ~JoystickHandler() override;

  /**
   * Poll all enabled joysticks
   * @return the updated states of all enabled joystick
   */
  InputList Poll() override;

  /**
   * Configure a joystick
   * @param id ID of the joystick that should be configured.
   */
  void SetConfig(int id, Input::Config) override;

 private:
  /**
   * Handle an axis event for a given joystick.
   * @param id the ID of the joystick to operate on.
   * @param axis the axis of the joystick to operate on.
   * @param value the new value of the axis.
   */
  void HandleAxis(unsigned id, unsigned axis, int value);

  /**
   * Handle a button event for a given joystick.
   * @param id the ID of the joystick to operate on.
   * @param button the button of the joystick to operate on.
   * @param pressed whether the button is pressed or released.
   */
  void HandleButton(unsigned id, unsigned button, bool pressed);

  /**
   * Handle a connection event for a given joystick.
   * @param id the ID of the joystick to operate on.
   * @param connected whether the joystick is connected or disconnected.
   */
  void HandleConnection(unsigned id, bool connected);

  /**
   * Enumerate and create input entries for all joysticks attached to this computer.
   */
  void ScanAll();

  /**
   * Scan extra information on an open joystick.
   * This will populate the button and axes in the corresponding joystick_inputs_ entry.
   * respectively.
   * @param id the ID of the joystick to scan
   */
  void Scan(unsigned id);

  /**
   * Open a libenjoy joystick
   * @param id the ID of the joystick to open
   */
  void Open(unsigned id);

  /**
   * Close a libenjoy joystick
   * @param id the ID of the joystick to close
   */
  void Close(unsigned id);

  //! raw pointer for the libenjoy context
  libenjoy_context* joy_context_;
  //! the state of each joystick, stored by ID
  std::map<unsigned, std::shared_ptr<Input>> joystick_inputs_;
  //! raw pointer for each libenjoy joystick, stored by ID
  std::map<unsigned, libenjoy_joystick*> joysticks_;
};
