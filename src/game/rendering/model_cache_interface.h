// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <filesystem>
#include <memory>

#include "game/rendering/model.h"

//! Forward Declared, defined in game/reader/reader_interface.h
class FileReaderInterface;

class ModelCacheInterface {
 public:
  virtual ~ModelCacheInterface() = default;

  virtual Model Register(const std::filesystem::path& file) = 0;

  [[nodiscard]] virtual const ModelData& GetData(ModelDataId id) const = 0;
};

namespace ModelCacheFactory {
std::unique_ptr<ModelCacheInterface> Create(std::unique_ptr<FileReaderInterface> file_reader);
}
