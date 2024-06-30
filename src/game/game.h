#pragma once

#include <raylib.h>

#include <memory>
#include <string>

//! Forward Declared, defined in game/input/input_manager_interface.h
class InputManagerInterface;

//! Forward Declared, defined in game/control/control_scheme_mapper.h
class ControlSchemeMapper;
//! Forward Declared, defined in game/world/rails.h
class Rails;
//! Forward Declared, defined in game/world/train.h
class Train;
//! Forward Declared, defined in game/script/script_interpreter.h
class ScriptInterpreter;

class Game {
 public:
  Game();
  ~Game();

  bool Loop();

 private:
  bool running_{true};
  bool show_debug_{false};
  Camera3D camera_{};
  std::unique_ptr<Rails> rails_;
  std::unique_ptr<Train> train_;

  std::unique_ptr<InputManagerInterface> input_;
  std::unique_ptr<ControlSchemeMapper> controls_mapper_;

  std::unique_ptr<ScriptInterpreter> script_;
};
