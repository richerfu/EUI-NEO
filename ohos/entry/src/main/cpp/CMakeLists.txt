cmake_minimum_required(VERSION 3.16)
project(eui_neo_ohos LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

get_filename_component(EUI_NEO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.." ABSOLUTE)
get_filename_component(EUI_NEO_DEFAULT_SDL3_SOURCE_DIR "${EUI_NEO_ROOT}/../SDL" ABSOLUTE)
set(SDL3_SOURCE_DIR "${EUI_NEO_DEFAULT_SDL3_SOURCE_DIR}" CACHE PATH "Path to the OHOS-enabled SDL3 source tree")
if(NOT EXISTS "${SDL3_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "SDL3_SOURCE_DIR does not point to an SDL3 source tree: ${SDL3_SOURCE_DIR}")
endif()

set(SDL_SHARED OFF CACHE BOOL "Build SDL3 shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL3 static library" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Build SDL3 tests" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build SDL3 test library" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Build SDL3 examples" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "Generate SDL3 install rules" FORCE)
add_subdirectory("${SDL3_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/sdl3" EXCLUDE_FROM_ALL)

set(EUI_BUILD_APPS OFF CACHE BOOL "Build EUI examples" FORCE)
set(EUI_BUILD_USER_APPS OFF CACHE BOOL "Build EUI user apps" FORCE)
set(EUI_BUILD_TEST_FIXTURES OFF CACHE BOOL "Build EUI fixtures" FORCE)
set(EUI_BUILD_SHARED OFF CACHE BOOL "Build EUI as a shared library" FORCE)
set(EUI_ENABLE_INSTALL OFF CACHE BOOL "Generate EUI install rules" FORCE)
set(EUI_DEPS_MODE bundled CACHE STRING "Use vendored EUI dependencies" FORCE)
set(EUI_WINDOW_BACKEND sdl3 CACHE STRING "Use SDL3" FORCE)
set(EUI_RENDER_BACKEND opengl CACHE STRING "Use OpenGL ES through SDL3" FORCE)
add_subdirectory("${EUI_NEO_ROOT}" "${CMAKE_BINARY_DIR}/eui-neo" EXCLUDE_FROM_ALL)

add_library(entry SHARED
    napi_init.cpp
    "${EUI_NEO_ROOT}/core/app/sdl3_app_main.cpp"
    "${EUI_NEO_ROOT}/examples/gallery.cpp"
)
target_compile_definitions(entry PRIVATE SDL_PLATFORM_OHOS=1)
target_link_libraries(entry PRIVATE
    eui::neo
    libace_napi.z.so
    libhilog_ndk.z.so
)
