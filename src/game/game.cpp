// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/game.h"

#include <absl/log/log.h>
#include <mp-units/format.h>

#include <format>
#include <fstream>
#include <memory>
#include <vector>

#include "camera/camera_interface.h"
#include "game/control/control_scheme_mapper.h"
#include "game/input/input_manager_interface.h"
#include "game/world/rails.h"
#include "game/world/train.h"
#include "game/world/units.h"
#include "platform/platform.h"
#include "third_party/raylib/raylib.h"
#include "utils/file_reader_interface.h"
#include "utils/version.h"

namespace {
using Control::TrainControls;
using FileType = FileReaderInterface::FileType;
const std::filesystem::path kRailsFile = "resources/test-track.bin";
constexpr auto kDebugTextFontsize = 20;

void DrawTextRightJustified(const char* text, const int pos_x, const int pos_y, const int fontsize) {
  Raylib::DrawText(text, pos_x - Raylib::MeasureText(text, fontsize), pos_y, fontsize, Raylib::BLACK);
}
}  // namespace

Game::Game()
    : rails_(std::make_unique<Rails>()),
      camera_(CameraFactory::Create()),
      input_(InputManagerFactory::Create(Platform::GetPlatform())),
      controls_mapper_(std::make_unique<ControlSchemeMapper>()),
      reader_(FileReaderFactory::Create(FileReaderFactory::Type::kWatcher)) {
  Raylib::SetTargetFPS(60);

  reader_->read(kRailsFile, FileType::kBinary, [this](std::ifstream stream) {
    if (!stream.is_open()) {
      LOG(ERROR) << "Failed to read rails file " << kRailsFile;
      return;
    }
    LOG(ERROR) << "Reading world data from " << kRailsFile;
    rails_ = World::Load(&stream);
  });

  train_ = std::make_unique<Train>(*rails_, Rails::Location{.segment = {3}}, 2);
}

Game::~Game() { Raylib::EnableCursor(); }

bool Game::Loop() {
  const auto controls = controls_mapper_->MapGameControls(input_->Poll());
  const Units::TimeDelta time = Raylib::GetFrameTime() * mp_units::si::second;
  if (controls.show_debug) {
    show_debug_ = !show_debug_;
  }
  train_->Control(controls.train_controls, time);
  camera_->Control(controls.camera_controls, time);
  camera_->Target(train_->GetCenterPoint());

  Raylib::BeginDrawing();
  Raylib::ClearBackground(Raylib::RAYWHITE);

  camera_->Activate();
  train_->Draw();
  if (show_debug_) {
    rails_->DrawDebug();
    train_->DrawDebug();
  }
  camera_->Deactivate();

  if (show_debug_) {
    // Draw top left status
    Raylib::DrawFPS(10, 10);
    Raylib::DrawText(std::format("ActiveInput={}", controls.input_name.empty() ? "None" : controls.input_name).c_str(),
                     10, 30, kDebugTextFontsize, Raylib::BLACK);
    Raylib::DrawText(
        std::format("Velocity={:.1f} km/h",
                    train_->Speed().numerical_value_in(mp_units::si::kilo<mp_units::si::metre> / mp_units::si::hour))
            .c_str(),
        10, 50, kDebugTextFontsize, Raylib::BLACK);

    // Draw bottom left version
    const auto bottom_align_pos = Raylib::GetScreenHeight() - 30;
    Raylib::DrawText(std::format("Version {}", Utils::GetVersion()).c_str(), 10, bottom_align_pos, kDebugTextFontsize,
                     Raylib::BLACK);

    // Draw bottom right controls
    const auto right_align_pos = Raylib::GetScreenWidth() - 10;
    DrawTextRightJustified("Accelerate:W", right_align_pos, bottom_align_pos - 60, kDebugTextFontsize);
    DrawTextRightJustified("Decelerate:S", right_align_pos, bottom_align_pos - 40, kDebugTextFontsize);
    DrawTextRightJustified("Reverse:R", right_align_pos, bottom_align_pos - 20, kDebugTextFontsize);
    DrawTextRightJustified("Toggle debug view:.", right_align_pos, bottom_align_pos, kDebugTextFontsize);
  }
  Raylib::EndDrawing();

  return !Raylib::WindowShouldClose();
}
