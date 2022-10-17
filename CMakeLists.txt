cmake_minimum_required(VERSION 3.23)

# The CMAKE_TOOLCHAIN_FILE variable is not used until the project() command is reached, at which point CMake looks for
# the named toolchain file relative to the build directory.
include(FetchContent)
FetchContent_Declare(toolchains DOWNLOAD_EXTRACT_TIMESTAMP URL https://github.com/cmake-embedded/toolchains/archive/refs/heads/main.zip)
FetchContent_MakeAvailable(toolchains)

project(firmware C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD 11)

add_subdirectory(board/stm32f072b-disco)
add_subdirectory(firmware)

