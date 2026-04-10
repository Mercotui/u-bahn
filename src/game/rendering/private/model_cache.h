// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <memory>
#include <unordered_map>

#include "game/rendering/model_cache_interface.h"
#include "game/rendering/private/mesh_packer.h"

//! Forward Declared, defined in game/reader/reader_interface.h
class FileReaderInterface;

class ModelCache final : public ModelCacheInterface {
 public:
  explicit ModelCache(std::unique_ptr<FileReaderInterface> file_reader,
                      std::unique_ptr<MeshPackerInterface> model_data_packer);
  ~ModelCache() override;

  Model Register(const std::filesystem::path& file) override;

  [[nodiscard]] const ModelData& GetData(ModelDataId id) const override;

 private:
  std::optional<ModelData> LoadData(std::ifstream stream) const;

  ModelDataId last_created_id_{};
  std::unordered_map<ModelDataId, ModelData> model_data_cache_;
  std::unique_ptr<FileReaderInterface> file_reader_;
  std::unique_ptr<MeshPackerInterface> mesh_packer_;
};
