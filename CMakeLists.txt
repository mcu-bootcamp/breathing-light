cmake_minimum_required(VERSION 3.19)

# The CMAKE_TOOLCHAIN_FILE variable is not used until the project() command is reached, at which point CMake looks for
# the named toolchain file relative to the build directory.
include(FetchContent)
FetchContent_Declare(toolchains URL https://codeload.github.com/cmake-embedded/toolchains/zip/refs/heads/main)
FetchContent_MakeAvailable(toolchains)

project(firmware C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD 11)

add_subdirectory(board/stm32f072b-disco)
add_subdirectory(firmware)

