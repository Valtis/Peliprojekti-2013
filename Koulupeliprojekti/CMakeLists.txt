cmake_minimum_required (VERSION 2.6)
project (PELI)

set (ENV{PKG_CONFIG_PATH}
  "/home/jtpakari/Software/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}"
)

set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBRARY_OUTPUT_PATH "bin")
set(CMAKE_CACHEFILE_DIR "cmake")

file(GLOB_RECURSE
  SOURCES
  "*.h"
  "*.cpp"
)

string(REGEX
  REPLACE
  "CMakeFiles/[^;]+;?"
  ""
  SOURCES
  "${SOURCES}"
)

find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

set(
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS}-std=c++0x -O3"
)

message (
  STATUS
  "${CMAKE_CXX_FLAGS}"
)

include_directories (
  SYSTEM
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
  ${SDL2_MIXER_INCLUDE_DIRS}
  ${PELI_SOURCE_DIR}/include
)

link_directories (
  ${SDL2_LIBRARY_DIRS}
  ${SDL2_TTF_LIBRARY_DIRS}
  ${SDL2_MIXER_LIBRARY_DIRS}
)

add_executable (
  Peli
  WIN32
  ${SOURCES}
)

target_link_libraries (
  Peli
  ${SDL2_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${SDL2_MIXER_LIBRARIES}
)

