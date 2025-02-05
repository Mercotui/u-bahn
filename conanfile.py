from conan import ConanFile
from conan.tools.cmake import cmake_layout


class UBahnRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("gtest/1.15.0")
        self.requires("benchmark/1.9.1")
        self.requires("abseil/20240722.0")
        self.requires("raylib/5.5")
        self.requires("glm/cci.20230113")
        self.requires("bezier/0.2.1")
        self.requires("mp-units/2.1.1")
        # self.requires("nlohmann_json/3.11.3")

    def layout(self):
        cmake_layout(self)
