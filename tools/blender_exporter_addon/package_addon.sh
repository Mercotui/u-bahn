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

# Copy the sources of the addon
cp -r ubahn_exporter_blender_addon/* dist/addon
# Remove any __pycache__ copy
rm -r dist/addon/__pycache__
# Replace the placeholder "init" with the real addon code
mv dist/addon/addon__init__.py dist/addon/__init__.py
# Package the entire addon including metadata and dependencies
flatpak run org.blender.Blender --command extension build --split-platforms --source-dir ./dist/addon --output-dir dist
