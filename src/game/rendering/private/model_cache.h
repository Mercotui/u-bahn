// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <memory>
#include <unordered_map>

#include "game/rendering/model_cache_interface.h"

//! Forward Declared, defined in game/reader/reader_interface.h
class FileReaderInterface;

class ModelCache final : public ModelCacheInterface {
 public:
  explicit ModelCache(std::unique_ptr<FileReaderInterface> file_reader);
  ~ModelCache() override;

  Model Load(const std::filesystem::path& file) override;

  [[nodiscard]] const ModelData& GetData(ModelDataId id) const override;

 private:
  ModelDataId last_created_id_{};
  std::unordered_map<ModelDataId, ModelData> model_data_cache_;
  std::unique_ptr<FileReaderInterface> file_reader_;
};
