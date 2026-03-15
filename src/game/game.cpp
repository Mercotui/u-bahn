// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/game.h"

#include <absl/log/log.h>
#include <mp-units/format.h>

#include <format>
#include <fstream>
#include <memory>
#include <vector>

#include "game/control/control_scheme_mapper.h"
#include "game/rendering/camera_interface.h"
#include "game/world/units.h"
#include "platform/platform.h"
#include "third_party/hello_triangle/hello_triangle.h"
#include "utils/file_reader_interface.h"
#include "utils/version.h"

namespace {
using Control::TrainControls;
using FileType = FileReaderInterface::FileType;
const std::filesystem::path kRailsFile = "resources/test-track.bin";
constexpr auto kDebugTextFontsize = 20;
}  // namespace

Game::Game()
    :  // rails_(std::make_unique<Rails>()),
      camera_(CameraFactory::Create()),
      // input_(InputManagerFactory::Create(Platform::GetPlatform())),
      controls_mapper_(std::make_unique<ControlSchemeMapper>()),
      reader_(FileReaderFactory::Create(FileReaderFactory::Type::kWatcher)) {
  reader_->Read(kRailsFile, FileType::kBinary, [this](std::ifstream stream) {
    if (!stream.is_open()) {
      LOG(ERROR) << "Failed to read rails file " << kRailsFile;
      return;
    }
    LOG(ERROR) << "Reading world data from " << kRailsFile;
    // rails_ = World::Load(&stream);

    if (!HelloTriangle::Init()) {
      LOG(FATAL) << "No renderer";
    }
  });

  // train_ = std::make_unique<Train>(*rails_, Rails::Location{.segment = {3}}, 2);
}

Game::~Game() { HelloTriangle::Shutdown(); }

bool Game::operator()(const Platform::LoopContext& context) {
  auto controls = controls_mapper_->MapGameControls(context.inputs);
  const Units::TimeDelta time = 0.01 * mp_units::si::second;
  controls.camera_controls.x = 1.0f;

  if (controls.show_debug) {
    show_debug_ = !show_debug_;
  }
  // train_->Control(controls.train_controls, time);
  camera_->Control(controls.camera_controls, time);
  // camera_->Target(train_->GetCenterPoint());

  // Raylib::BeginDrckground(Raylib::RAYWHITE);
  const auto transform = camera_->Transform();
  HelloTriangle::Draw(transform);
  // train_->Draw();
  if (show_debug_) {
    // rails_->DrawDebug();
    // train_->DrawDebug();
  }

  if (show_debug_) {
    // Draw top left status
    // Raylib::DrawFPS(10, 10);
    // Raylib::DrawText(std::format("ActiveInput={}", controls.input_name.empty() ? "None" :
    // controls.input_name).c_str(),
    //                  10, 30, kDebugTextFontsize, Raylib::BLACK);
    // Raylib::DrawText(
    //     std::format("Velocity={:.1f} km/h",
    //                 train_->Speed().numerical_value_in(mp_units::si::kilo<mp_units::si::metre> / mp_units::si::hour))
    //         .c_str(),
    //     10, 50, kDebugTextFontsize, Raylib::BLACK);

    // Draw bottom left version
    // const auto bottom_align_pos = Raylib::GetScreenHeight() - 30;
    // Raylib::DrawText(std::format("Version {}", Utils::GetVersion()).c_str(), 10, bottom_align_pos,
    // kDebugTextFontsize,
    //                  Raylib::BLACK);

    // Draw bottom right controls
    // const auto right_align_pos = Raylib::GetScreenWidth() - 10;
    // DrawTextRightJustified("Accelerate:W", right_align_pos, bottom_align_pos - 60, kDebugTextFontsize);
    // DrawTextRightJustified("Decelerate:S", right_align_pos, bottom_align_pos - 40, kDebugTextFontsize);
    // DrawTextRightJustified("Reverse:R", right_align_pos, bottom_align_pos - 20, kDebugTextFontsize);
    // DrawTextRightJustified("Toggle debug view:.", right_align_pos, bottom_align_pos, kDebugTextFontsize);
  }
  // Raylib::EndDrawing();

  return true;  // !Raylib::WindowShouldClose();
}
