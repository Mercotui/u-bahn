#!/usr/bin/env bash
#
# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT
#

# Log commands
set -x

# Set up a clean packaging dir
mkdir -p dist
rm -r dist/addon
rm "dist/*.zip"

# Exit on failure
set -e

# Download our dependencies, following the documentation:
# https://docs.blender.org/manual/en/4.4/advanced/extensions/python_wheels.html
poetry export -o dist/requirements.txt
poetry run pip download -r dist/requirements.txt -d dist/addon/wheels

# Package the entire addon including metadata and dependencies
cp src/* dist/addon
flatpak run org.blender.Blender --command extension build --split-platforms --source-dir ./dist/addon --output-dir dist
