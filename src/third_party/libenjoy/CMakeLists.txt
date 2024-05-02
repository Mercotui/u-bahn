project(enjoy VERSION 0.1 LANGUAGES C)

add_library(enjoy STATIC
        src/libenjoy.c
        src/libenjoy_linux.c
)

target_include_directories(enjoy PRIVATE src)
target_include_directories(enjoy PUBLIC include)

#  HEADERS += $$PWD/src/libenjoy.h \
#    $$PWD/src/libenjoy_p.h
#
#        INCLUDEPATH += $$PWD/src
#
#        win32 {
#        SOURCES += $$PWD/src/libenjoy_win32.c
#        HEADERS += $$PWD/src/libenjoy_win32.h
#        }
#
#        linux-* {
#
#        HEADERS += $$PWD/src/libenjoy_linux.h
#        }