// SPDX-FileCopyrightText: 2022 - 2025 Sean Apeler
// SPDX-License-Identifier: MIT
// This is a modified version of the GltfFileStream from
// https://github.com/spnda/fastgltf/blob/v0.9.0/include/fastgltf/core.hpp
#pragma once

#include <fastgltf/core.hpp>
#include <fstream>

namespace fastgltf {
class GltfStreamReader final : public GltfDataGetter {
  std::ifstream fileStream;
  std::vector<std::ifstream::char_type> buf;

  std::size_t fileSize{};

 public:
  explicit GltfStreamReader(std::ifstream stream);
  ~GltfStreamReader() noexcept override = default;

  [[nodiscard]] bool isOpen() const;

  void read(void* ptr, std::size_t count) override;

  [[nodiscard]] span<std::byte> read(std::size_t count, std::size_t padding) override;

  void reset() override;

  [[nodiscard]] std::size_t bytesRead() override;

  [[nodiscard]] std::size_t totalSize() override;
};
}  // namespace fastgltf
