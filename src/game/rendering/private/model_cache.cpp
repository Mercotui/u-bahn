// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/model_cache.h"

#include <absl/log/log.h>

#include <fastgltf/core.hpp>
#include <fastgltf/stream_reader.hpp>
#include <memory>
#include <optional>
#include <utility>

#include "game/rendering/private/model_data_packer.h"
#include "utils/file_reader_interface.h"

namespace {
std::optional<ModelData> LoadData(std::ifstream stream) {
  fastgltf::GltfStreamReader stream_reader(std::move(stream));
  fastgltf::Parser parser;
  auto expected_asset = parser.loadGltfBinary(stream_reader, "");
  if (const auto error = expected_asset.error(); error != fastgltf::Error::None) {
    LOG(ERROR) << "Loading GLTF asset resulted in error: " << fastgltf::getErrorMessage(error);
    return std::nullopt;
  }
  const auto& asset = expected_asset.get();
  if (const auto error = fastgltf::validate(asset); error != fastgltf::Error::None) {
    LOG(WARNING) << "GLTF asset validation found issue: " << fastgltf::getErrorMessage(error);
  }
  return ModelDataPacker::PackBuffers(asset);
}
}  // namespace

ModelCache::ModelCache(std::unique_ptr<FileReaderInterface> file_reader) : file_reader_(std::move(file_reader)) {}

/// Declared here for forward declaration reasons
ModelCache::~ModelCache() = default;

Model ModelCache::Load(const std::filesystem::path& file) {
  ModelDataId id = ++last_created_id_;
  file_reader_->Read(file, FileReaderInterface::FileType::kBinary, [this, file = file, id](std::ifstream file_stream) {
    if (auto data = LoadData(std::move(file_stream))) {
      model_data_cache_.emplace(id, data.value());
    } else {
      LOG(ERROR) << "Failed to load GLTF asset from file: " << file;
    }
  });
  return Model{.data_id = id, .material = Material::kPlain, .transform = glm::mat4(1.0f)};
}  // namespace

std::unique_ptr<ModelCacheInterface> ModelCacheFactory::Create(std::unique_ptr<FileReaderInterface> file_reader) {
  return std::make_unique<ModelCache>(std::move(file_reader));
}

const ModelData& ModelCache::GetData(const ModelDataId id) const {
  const auto data = model_data_cache_.find(id);
  if (data == model_data_cache_.end()) {
    LOG(ERROR) << "No ModelData entry found for ID: " << id;
    static ModelData kNoData{};
    return kNoData;
  }
  return data->second;
}
