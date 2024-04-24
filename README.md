# U-Bahn

U-Bahn game

## Dependencies

This project requires OpenGL ES 3.0 or similar level GL/WebGL.

Other dependencies for this project are managed via conan, to install conan on fedora linux use:

```bash
sudo yum install pipx
pipx install conan
conan profile detect --force
conan install --pr:b=default --pr:h=default  ..
```

## Build

To build this project use:

```bash
# install dependencies
conan install -u -pr:b=default -pr:h=default --build=missing .
# configure cmake
cmake --preset conan-default
# build for debug
cmake --build --preset conan-debug
# or build for release
cmake --build --preset conan-release
```
