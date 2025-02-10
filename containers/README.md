# CI Containers

To build and publish the CI container for this project run the following commands:

```bash
podman build -f build.containerfile -t ghcr.io/mercotui/ubahn_buildenv:1.x
podman push ghcr.io/mercotui/ubahn_buildenv:1.x
```

To test the CI container locally you can use:

```bash
podman run -it -v ..:/u-bahn:z <image ID>
cd u-bahn
conan install -u -pr:b=default -pr:h=profiles/emscripten.profile --build=missing --settings=build_type=Release .
cmake --preset conan-emscripten-release
cmake --build --preset conan-emscripten-release --target u-bahn
```
