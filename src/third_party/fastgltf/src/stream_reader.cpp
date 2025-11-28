// SPDX-FileCopyrightText: 2022 - 2025 Sean Apeler
// SPDX-License-Identifier: MIT
// This is a modified version of the GltfFileStream from https://github.com/spnda/fastgltf/blob/v0.9.0/src/io.cpp

#include <fastgltf/stream_reader.hpp>

namespace fastgltf {
GltfStreamReader::GltfStreamReader(std::ifstream stream) : fileStream(std::move(stream)) {
  // Get start of file
  fileStream.seekg(0, std::ifstream::beg);
  fileSize = fileStream.tellg();
  // Get end of file
  fileStream.seekg(0, std::ios::end);
  fileSize = static_cast<std::size_t>(fileStream.tellg()) - fileSize;

  // Reset to start of file
  fileStream.seekg(0, std::ifstream::beg);
}

bool GltfStreamReader::isOpen() const { return fileStream.is_open(); }

void GltfStreamReader::read(void* ptr, std::size_t count) {
  fileStream.read(static_cast<char*>(ptr), static_cast<std::streamsize>(count));
}

span<std::byte> GltfStreamReader::read(std::size_t count, std::size_t padding) {
  static_assert(sizeof(decltype(buf)::value_type) == sizeof(std::byte));

  buf.resize(count + padding);
  fileStream.read(buf.data(), static_cast<std::streamsize>(count));

  return span(reinterpret_cast<std::byte*>(buf.data()), buf.size());
}

void GltfStreamReader::reset() { fileStream.seekg(0, std::ifstream::beg); }

std::size_t GltfStreamReader::bytesRead() { return fileStream.tellg(); }

std::size_t GltfStreamReader::totalSize() { return fileSize; }
}  // namespace fastgltf
