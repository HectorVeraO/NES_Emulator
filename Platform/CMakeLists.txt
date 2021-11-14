set(PLATFORM_LIB_HEADER_DIRS
        Canvas)

set(PLATFORM_LIB_HEADER_FILES
        Canvas/Canvas.h)

set(PLATFORM_LIB_SOURCE_FILES
        Canvas/Canvas.cpp)

include_directories(Canvas)
include_directories(${sdl2_SOURCE_DIR}/include ${sdl2_SOURCE_DIR})

add_library(PLATFORM_LIB STATIC ${PLATFORM_LIB_SOURCE_FILES} ${PLATFORM_LIB_HEADER_FILES})
