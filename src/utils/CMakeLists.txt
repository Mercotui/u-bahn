add_library(version STATIC private/version.cpp)
add_library(Version::Version ALIAS version)

target_compile_definitions(version PRIVATE UBAHN_VERSION="${UBAHN_VERSION}")
