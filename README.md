# breathing-light

## Steps to build

1. Requirements
    - CMake, v3.19+
    - Arm GNU Toolchain, v7+
      - 11.2 is not recommended because of defect "Illegal instruction": https://bugs.linaro.org/show_bug.cgi?id=5825
2. Just run CMake to build
    ```shell
    mkdir build
    cmake --toolchain _deps/toolchains-src/gcc/arm-none-eabi/cortex-m0.cmake -B build
    cmake --build build --target dist
    ```

> ###### Note
> 
> When using IDE instead of command line, be aware of the key of build is to specify toolchain file for cross-compile as:
> `--toolchain _deps/toolchains-src/gcc/arm-none-eabi/cortex-m0.cmake`
> 
> This toolchain file is not part of this repo, it will download automatically from GitHub during first time configure.