# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT
from quay.io/centos/centos:stream10

# Enable EPEL
run dnf config-manager --set-enabled crb && \
    dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-10.noarch.rpm

# Install build tools followed by app dependencies
run dnf update -y && \
    dnf install -y clang ninja-build cmake pipx \
        mesa-libGL-devel xkeyboard-config-devel && \
    dnf clean all

# Append pipx install dir to path
env PATH="$PATH:/root/.local/bin"

# Install conan and poetry
run pipx install conan poetry
