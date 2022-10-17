# breathing-light

## Steps to build

1. Requirements
    - CMake, v3.19+
    - Arm GNU Toolchain, v7+
      - 11.2 is not recommended because of defect "Illegal instruction": https://bugs.linaro.org/show_bug.cgi?id=5825
2. Just run CMake to build
    ```shell
    mkdir build
    cmake --toolchain cmake/cortex-m0.cmake -B build
    cmake --build build --target dist
    ```
