cmake_minimum_required(VERSION 3.11...3.23)


project(
        WDNES
        VERSION 0.0.1
        DESCRIPTION "NES emulator"
        LANGUAGES CXX
)


#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_VERBOSE_MAKEFILE OFF)
    #set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

    set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
#endif()


include(FetchContent)

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()


FetchContent_Declare(
        sdl2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG 25f9ed8
)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG 76fb40d
)

FetchContent_MakeAvailable(sdl2 spdlog)

set(dummy "This is a dummy string")
message("Main CMakeLists ${dummy}")

# Compiled library code
add_subdirectory(src)

# Executable code
#add_subdirectory(apps)


# Testing onlye available if this is the main app
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    #add_subdirectory(tests)
endif()
