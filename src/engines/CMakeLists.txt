message("src/engines/CMakeLists ${dummy}")

set(LibCanvasHeaders canvas/Canvas.h)
set(LibCanvasSources canvas/Canvas.cpp)

# TODO: There is possibly another way to "share" the headers location among CMakeLists children
set(LibCanvasInclude "${PROJECT_SOURCE_DIR}/src/engines/canvas" CACHE INTERNAL "")

add_library(LibCanvas STATIC ${LibCanvasSources} ${LibCanvasHeaders})

target_include_directories(LibCanvas PUBLIC
        canvas)
#target_include_directories(LibCanvas PRIVATE
#        "${sdl2_SOURCE_DIR}/include")

target_link_libraries(LibCanvas SDL2-static)