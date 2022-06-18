message("src/CMakeLists ${dummy}")

add_subdirectory(engines)
add_subdirectory(systems)

add_executable(WDNES main.cpp)

target_include_directories(WDNES PRIVATE
        "${spdlog_SOURCE_DIR}/include")

target_link_libraries(WDNES PRIVATE spdlog LibNes mingw32 SDL2-static SDL2main)

target_compile_definitions(LibNes PUBLIC
        DISABLE_6502_LOGS=1
        DISABLE_6502_BUGS=0)
