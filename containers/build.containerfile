# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT

from quay.io/fedora/fedora-minimal:43

# TODO(Menno 22.11.2025) Switched from quay.io/centos/centos:stream10 to fedora because of missing SDL3-devel
#  Track issue at https://issues.redhat.com/browse/RHEL-130477
# from quay.io/centos/centos:stream10

# Enable EPEL
# run dnf config-manager --set-enabled crb && \
#     dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-10.noarch.rpm

# Install build tools followed by app dependencies
run dnf update -y && \
    dnf install -y clang ninja-build cmake pipx git \
        systemd-devel mesa-libGLES-devel mesa-libEGL-devel SDL3-devel && \
    dnf clean all

# Append pipx install dir to path
env PATH="$PATH:/root/.local/bin"

# Install conan and poetry
run pipx install conan poetry
