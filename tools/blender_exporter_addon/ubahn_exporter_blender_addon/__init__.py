#  SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
#  SPDX-License-Identifier: MIT
#
# This is just a placeholder file!
# This file is used for developing the blender plugin with standard python tooling.
# Everything in this file should be non-blender specific, so that it can be ran outside of blender.
# The actual plugin code that will run in blender resides in addon__init__.py.
from . import export_rails

if __name__ == "__main__":
    export_rails.convert([])
