#!/usr/bin/env bash
# File adapted from https://github.com/raysan5/raylib/wiki/Working-for-Android-(on-Linux)#build

# exit this script if any command returns non-zero
set -e
# enable recursive glob expansion, i.e. ./**.class will search all sub-directories for .class files
shopt -s globstar

# Set default value if not already assigned. Feel free to assign them before calling the package script
echo "--- Using sdk: ${SDK_ROOT:=${HOME}/Android/Sdk}"
echo "--- Using apk packaging dir: ${APK_PACKAGING_DIR:=${PWD}/apk_build}"
echo "--- Using main library: ${MAIN_LIB:=build/android/Release/src/libu-bahn.so}"

# Folder used to construct the APK contents. AAPT requires it's input-files to already be in the right relative paths.
APK_ROOT="${APK_PACKAGING_DIR}/root"
# Folder containing the sdk buildtools
BUILD_TOOLS="${SDK_ROOT}/build-tools/34.0.0"

echo "--- Clearing apk packaging dir"
rm -rf "${APK_PACKAGING_DIR}"
mkdir "${APK_PACKAGING_DIR}"

echo "--- Creating APK and R.java"
"${BUILD_TOOLS}/aapt" package -f -m \
  -S src/platform/android/resources -M src/platform/android/AndroidManifest.xml \
  -I "${SDK_ROOT}/platforms/android-34/android.jar" -J "${APK_PACKAGING_DIR}"

echo "--- Compiling NativeLoader.java into jvm-bytecode"
javac -verbose -source 1.8 -target 1.8 \
  -d "${APK_PACKAGING_DIR}/obj" \
  -bootclasspath jre/lib/rt.jar \
  -classpath "${SDK_ROOT}/platforms/android-34/android.jar":"${APK_PACKAGING_DIR}/obj" \
  -sourcepath src "${APK_PACKAGING_DIR}"/**/R.java \
  src/platform/android/NativeLoader.java

echo "--- Compiling the jvm-bytecode into dalvik-bytecode"
mkdir "${APK_PACKAGING_DIR}/dex/"
# d8 can also desugar our JVM bytecode to transpile java features that our android version does not support,
# but with our tiny java source, we don't need that functionality.
"${BUILD_TOOLS}/d8" --no-desugaring --output "${APK_PACKAGING_DIR}/dex/" "${APK_PACKAGING_DIR}"/obj/**/*.class

echo "--- Creating APK again?"
"${BUILD_TOOLS}/aapt" package -f -m -A resources \
  -S src/platform/android/resources -M src/platform/android/AndroidManifest.xml \
  -I "${SDK_ROOT}/platforms/android-34/android.jar" -F "${APK_PACKAGING_DIR}/game.apk" -J "${APK_PACKAGING_DIR}" "${APK_PACKAGING_DIR}/dex/"

echo "--- Adding any resources to APK, feel free to add more if needed"
mkdir -p "${APK_ROOT}/lib/arm64-v8a/"
cp "${MAIN_LIB}" "${APK_ROOT}/lib/arm64-v8a/libmain.so"
#cp -r resources "${APK_ROOT}/assets

cd "${APK_ROOT}"
"${BUILD_TOOLS}/aapt" add "${APK_PACKAGING_DIR}/game.apk" lib/arm64-v8a/libmain.so
#"${BUILD_TOOLS}/aapt" add "${APK_PACKAGING_DIR}/game.apk" assets/
cd -

echo "--- Signing APK"
# Every APK needs to be signed, even dev builds, otherwise the phone will refuse to install it.
# Note: if you changed the storepass and keypass in the setup process, change them here too
jarsigner -keystore u-bahn.keystore -storepass admintest -keypass admintest \
  -signedjar "${APK_PACKAGING_DIR}/game.signed.apk" \
  "${APK_PACKAGING_DIR}/game.apk" projectKey

echo "--- Optimizing APK"
"${BUILD_TOOLS}/zipalign" -f 4 "${APK_PACKAGING_DIR}/game.signed.apk" "${APK_PACKAGING_DIR}/game.optimized.apk"
cp "${APK_PACKAGING_DIR}/game.optimized.apk" game.apk
