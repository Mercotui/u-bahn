// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <memory>

#include "platform/platform.h"

//! Forward Declared, defined in game/camera/camera_interface.h
class CameraInterface;
//! Forward Declared, defined in game/control/control_scheme_mapper.h
class ControlSchemeMapper;
//! Forward Declared, defined in game/world/rails.h
class Rails;
//! Forward Declared, defined in game/world/train.h
class Train;
//! Forward Declared, defined in game/reader/reader_interface.h
class FileReaderInterface;

class Game : public Platform::MainLoop {
 public:
  Game();
  ~Game() override;

  bool operator()(const Platform::LoopContext& context) override;

 private:
  bool running_{true};
  bool show_debug_{true};
  // std::unique_ptr<Rails> rails_;
  // std::unique_ptr<Train> train_;

  std::unique_ptr<CameraInterface> camera_;
  std::unique_ptr<ControlSchemeMapper> controls_mapper_;
  std::unique_ptr<FileReaderInterface> reader_;
};
