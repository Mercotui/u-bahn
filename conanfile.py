from conan import ConanFile
from conan.tools.cmake import cmake_layout


class UBahnRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def configure(self):
        self.options["glfw"].with_x11 = False
        self.options["glfw"].with_wayland = True
        self.options["xkbcommon"].with_x11 = False
        self.options["xkbcommon"].with_wayland = True

    def requirements(self):
        self.requires("gtest/1.14.0")
        self.requires("abseil/20240116.2")

        # TODO(Menno 23.04.2024) This reference is my custom package, I intend to upstream it:
        #   https://github.com/Mercotui/conan-center-index/commit/e367d9c73bef85e63a62ba572e0e4d1fec9cb948
        #   To create raylib package without x11:
        #    -o glfw/*:with_x11=False -o glfw/*:with_wayland=True
        #    -o xkbcommon/*:with_x11=False -o xkbcommon/*:with_wayland=True
        #   To export package with out building it:
        #    cd conan-center-index/recipes/raylib/all$
        #    conan export . --version "5.x"
        self.requires("raylib/5.x#d7043addbfc85a89e0081903f6f7d359")

        self.requires("glm/cci.20230113")
        self.requires("bezier/0.2.1")
        self.requires("mp-units/2.1.1")
        # self.requires("nlohmann_json/3.11.3")

    def layout(self):
        cmake_layout(self)
