#  SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
#  SPDX-License-Identifier: MIT
from conan import ConanFile
from conan.tools.cmake import cmake_layout


class UBahnRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("gtest/1.15.0")
        self.requires("benchmark/1.9.1")
        self.requires("abseil/20240116.2")
        self.requires("protobuf/5.27.0")
        self.requires("raylib/5.5")
        self.requires("glm/cci.20230113")
        self.requires("bezier/0.2.1")
        self.requires("mp-units/2.1.1")
        self.requires("watcher/0.12.0")

    def build_requirements(self):
        self.tool_requires("protobuf/5.27.0")

    def layout(self):
        cmake_layout(self)
