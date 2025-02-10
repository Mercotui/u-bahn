# Building U-Bahn

These are the instructions to build U-Bahn from source.

## Dependencies

This project requires OpenGL ES 3.0 or similar level GL/WebGL.

Other dependencies for this project are managed via conan, to install conan on fedora linux use:

```bash
sudo yum install pipx
pipx install conan
conan profile detect --force
conan install --pr:b=default --pr:h=default  ..
```

## Build for Desktop

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

## Build for Web

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

## Build for Android

To build an Android APK follow these steps:

### Prerequisites

Conan can manage the NDK for us, but not the SDK.
The SDK download is hidden at the bottom of the [studio download](https://developer.android.com/studio) page,
the command line tools zip file. The easiest way to use the sdk is to put it in a path like: `cmdline-tools/latest/`.
For example:

```bash
$ ls ~/android/Sdk/cmdline-tools/latest/
bin  lib  NOTICE.txt  source.properties
```

To install the desired SDK, use the sdk manager like:

```bash
~/android/Sdk/cmdline-tools/latest/bin/sdkmanager --install "platforms;android-34" "build-tools;34.0.0"
```

We need to generate a pair of keys for signing our development builds. I would highly recommend not using these keys for
anything other than development builds.

```bash
keytool -genkeypair -validity 1000 -dname "CN=Mercotui,O=Android,C=ES" -keystore u-bahn.keystore -storepass 'admintest' -keypass 'admintest' -alias projectKey -keyalg RSA
```

### Build

We can then install our conan dependencies and build the shared library:

```bash
conan install -u -pr:b=default -pr:h=profiles/android.profile --build=missing  --settings=build_type=Release .
# configure cmake
cmake -G Ninja --preset conan-android-release
# build library for release
cmake --build --preset conan-android-release --target u-bahn
# build APK
./src/platform/android/package_apk.sh
```

The resulting APK is then `./game.apk`.

## Using the APK

First to install the platform tools and to start the daemon:

```bash
~/android/Sdk/cmdline-tools/latest/bin/sdkmanager --install "platform-tools"
sudo ~/android/Sdk/platform-tools/adb start-server
```

Then connect your phone to the PC see [this](https://developer.android.com/tools/adb) for more information, check the
connection with:

```bash
~/android/Sdk/platform-tools/adb devices -l
```

And finally to install the APK run:

```bash
~/android/Sdk/platform-tools/adb install ./game.apk
```
