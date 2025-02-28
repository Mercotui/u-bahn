// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "utils/private/file_reader.h"

#include <absl/log/log.h>

#include <fstream>
#include <memory>
#include <vector>
#include <wtr/watcher.hpp>

#include "platform/platform.h"

namespace {
void OpenFile(const std::filesystem::path& file_path, const FileReaderInterface::FileType type,
              const FileReaderInterface::ReadCb& cb) {
  const auto mode = type == FileReaderInterface::FileType::kBinary ? std::ios::binary | std::ios::in : std::ios::in;
  cb(std::ifstream(file_path, mode));
}
}  // namespace

std::unique_ptr<FileReaderInterface> FileReaderFactory::Create(const Type type) {
  // Platforms other than Desktop always use simple reader
  if (Platform::GetPlatform() != Platform::Type::kDesktop) {
    return std::make_unique<FileReader>();
  }

  switch (type) {
    case Type::kWatcher: {
      return std::make_unique<WatcherFileReader>();
    }
    default: {
      LOG(ERROR) << "Unknown reader type, using Simple Reader";
      [[fallthrough]];
    }
    case Type::kSimple: {
      return std::make_unique<FileReader>();
    }
  }
}

void FileReader::read(const std::filesystem::path& file, const FileType type, const ReadCb cb) {
  const auto file_absolute = std::filesystem::absolute(file);
  OpenFile(file_absolute, type, cb);
}

void WatcherFileReader::read(const std::filesystem::path& file, const FileType type, ReadCb cb) {
  const auto file_absolute = std::filesystem::absolute(file);

  auto event_cb = [file_absolute, type, cb](const wtr::event& event) {
    if (event.path_name != file_absolute) {
      return;
    }
    if (event.path_type != wtr::event::path_type::file) {
      LOG(WARNING) << "Got event for " << event.path_name << " which is is not a file, but instead " << event.path_type;
      return;
    }

    if (event.effect_type != wtr::event::effect_type::modify) {
      LOG(INFO) << "Uninteresting event happened for " << event.path_name << " event type: " << event.effect_type;
      return;
    }

    OpenFile(file_absolute, type, cb);
  };

  // Set up a watch
  watchers_.emplace_back(new wtr::watch(file.parent_path(), event_cb));
  // Trigger the first read
  OpenFile(file_absolute, type, cb);
}
