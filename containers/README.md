# CI Containers

To build and publish the CI container for this project run the following commands:

```bash
podman login ghcr.io
podman build -f build.containerfile -t ghcr.io/mercotui/ubahn_buildenv:xx
podman push ghcr.io/mercotui/ubahn_buildenv:xx
```

Note that in `ubahn_buildenv:xx` the `xx` part should be the CALVER version following YYYY.MM.PP scheme.

To test the CI container locally you can use:

```bash
podman run -it -v ..:/u-bahn:z ghcr.io/mercotui/ubahn_buildenv:xx
pipx install conan
cd u-bahn
conan install -u -pr:b=default -pr:h=profiles/emscripten.profile --build=missing --settings=build_type=Release .
cmake --preset conan-emscripten-release
cmake --build --preset conan-emscripten-release --target u-bahn
```
