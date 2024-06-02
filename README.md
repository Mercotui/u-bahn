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

To build this project for desktop use:

```bash
# install dependencies
conan install -u -pr:b=default -pr:h=default --build=missing --settings=build_type=Debug .
conan install -u -pr:b=default -pr:h=default --build=missing --settings=build_type=Release .

# configure cmake
cmake --preset conan-default
# build for debug
cmake --build --preset conan-debug
# or build for release
cmake --build --preset conan-release
```

To build for web:

```bash
conan install -u -pr:b=default -pr:h=profiles/emscripten.profile --build=missing  --settings=build_type=Release .
cmake --preset conan-default
cmake --build --preset conan-release
```

To run the build in your browser use any webserver, I prefer to use caddy:

```bash
caddy file-server --listen localhost:8000 -r build/emscripten/Release/src
```

Then visit the url:

```text
http://localhost:8000/u-bahn.html
```
