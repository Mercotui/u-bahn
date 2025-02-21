# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    message(STATUS "packaging.cmake: not yet implemented")
elseif (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CPACK_GENERATOR "TGZ")
    set(CPACK_ARCHIVE_FILE_EXTENSION "tar.gz")
    set(CPACK_PACKAGE_FILE_NAME "u-bahn-web-${UBAHN_VERSION}")
else ()
    message(STATUS "packaging.cmake: not yet implemented")
endif ()

set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/dist")
set(CPACK_PACKAGE_VERSION ${UBAHN_VERSION})
set(CPACK_VERBATIM_VARIABLES TRUE)

include(CPack)
