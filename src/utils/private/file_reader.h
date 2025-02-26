// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <filesystem>
#include <vector>
#include <wtr/watcher.hpp>

#include "utils/file_reader_interface.h"

/**
 * Basic reader implementation. Will read a file once.
 */
class FileReader final : public FileReaderInterface {
 public:
  /**
   * Read the specified file, call callback to handle contents
   * @param file The file to read
   * @param type The file type, determines open mode
   * @param cb The callback that will handle the file content
   */
  void read(const std::filesystem::path& file, FileType type, ReadCb cb) override;
};

class WatcherFileReader final : public FileReaderInterface {
 public:
  /**
   * Read the specified file and watch it for changes, reread if/when it changes.
   * @param file The file to read and watch
   * @param type The file type, determines open mode
   * @param cb The callback to call each time the file was read
   */
  void read(const std::filesystem::path& file, FileType type, ReadCb cb) override;

 private:
  std::vector<std::unique_ptr<wtr::watch>> watchers_;
};
