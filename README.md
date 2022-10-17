# breathing-light

## Steps to build

1. Requirements
    - CMake, v3.23+
    - Arm GNU Toolchain, v7+
      - 11.2 is not recommended because of defect "Illegal instruction": https://bugs.linaro.org/show_bug.cgi?id=5825
2. Just run CMake to build
    ```shell
    cmake --preset stm32f072b-disco
    cmake --build --preset stm32f072b-disco
    ```

> ###### Note
> 
> When using IDE instead of command line, be aware of the key of build is to specify preset as:
> `--preset stm32f072b-disco`
> 
> The preset configures the toolchain file for cross-compilation which will be downloaded automatically from GitHub during first time configure.