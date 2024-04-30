#pragma once

#include <memory>
#include <vector>

#include "game/input/input.h"
#include "platform/platform.h"

class InputManagerInterface {
 public:
  virtual ~InputManagerInterface() = default;

  [[nodiscard]] virtual std::vector<Input::Info> ListInputs() const = 0;
  virtual void Poll() = 0;

  virtual void SetConfig(int id, Input::Config) = 0;
};

namespace InputManagerFactory {
std::unique_ptr<InputManagerInterface> Create(Platform::Type platform);
}