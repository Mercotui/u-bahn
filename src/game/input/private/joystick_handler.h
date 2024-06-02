#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "game/input/input.h"
#include "game/input/input_manager_interface.h"
#include "game/input/private/input_axis_helpers.h"

//! Forward declared, defined in libenjoy.h
struct libenjoy_context;
//! Forward declared, defined in libenjoy.h
struct libenjoy_joystick;

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
   * @param config config to apply to the joystick
   */
  void SetConfig(int id, Input::Config config) override;

 private:
  /**
   * Handle an axis event for a given joystick.
   * @param id the ID of the joystick to operate on.
   * @param axis the axis of the joystick to operate on.
   * @param value the new value of the axis.
   * @param time_point time when this sample was read
   */
  void HandleAxis(unsigned id, unsigned axis, int value, InputAxisHelpers::SampleTimePoint time_point);

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
   * Rescan for joysticks and reconnect them if needed
   */
  void CheckReconnect();

  /**
   * Enumerate and create input entries for all joysticks attached to this computer.
   */
  void ScanAll();

  /**
   * Scan extra information on an open joystick.
   * This will populate the button and axes in the corresponding joystick_inputs_ entry..
   * @param id the ID of the joystick to scan
   */
  void ScanFeatures(unsigned id);

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

  //! Activity detection helper for axes
  using ActivityDetector = InputAxisHelpers::ActivityDetector;
  //! key type to store axes into maps
  struct JoystickAxisKey {
    unsigned id;
    unsigned axis;

    [[nodiscard]] bool operator==(const JoystickAxisKey& other) const { return id == other.id && axis == other.axis; }
    struct Hasher {
      std::size_t operator()(const JoystickAxisKey& instance) const {
        std::size_t h1 = std::hash<unsigned>()(instance.id);
        std::size_t h2 = std::hash<unsigned>()(instance.axis);
        return h1 ^ (h2 << 1);
      }
    };
  };

  //! raw pointer for the libenjoy context
  libenjoy_context* joy_context_;
  //! the state of each joystick, stored by ID
  std::unordered_map<unsigned, std::shared_ptr<Input>> joystick_inputs_;
  //! activity detectors for each axis, stored by pair of joystick ID and axis ID.
  std::unordered_map<JoystickAxisKey, ActivityDetector, JoystickAxisKey::Hasher> axis_activity_;
  //! raw pointer for each libenjoy joystick, stored by ID
  std::unordered_map<unsigned, libenjoy_joystick*> joysticks_;
  //! set of joysticks that should be reopened if they are reconnected
  std::unordered_set<unsigned> reconnect_joysticks_;
  //! time since last joystick scan
  std::chrono::time_point<std::chrono::steady_clock> scan_timestamp_;
};
