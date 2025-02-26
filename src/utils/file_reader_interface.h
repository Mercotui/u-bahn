// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <filesystem>
#include <functional>
#include <memory>

/**
 * An abstract class that handles loading of json files
 */
class FileReaderInterface {
 public:
  /**
   * Virtual base destructor for destruction of superclasses.
   */
  virtual ~FileReaderInterface() = default;

  /**
   * Type of file
   */
  enum class FileType { kText, kBinary };

  /**
   * Read callback format
   */
  using ReadCb = std::function<void(std::ifstream)>;

  /**
   * Read the specified file, call callback to handle contents
   * @param file The file to read
   * @param type The file type, determines open mode
   * @param cb The callback that will handle the file content
   */
  virtual void read(const std::filesystem::path& file, FileType type, ReadCb cb) = 0;
};

namespace FileReaderFactory {
enum class Type {
  //! Will read a file once.
  kSimple,
  //! Will reread a file if/when it changes. Note: the Read callback must live as long as the reader instance
  kWatcher
};

/**
 * Create a FileReader instance of the desired type
 * @param type the desired type
 * @return the file reader instance
 */
std::unique_ptr<FileReaderInterface> Create(Type type);
}  // namespace FileReaderFactory
