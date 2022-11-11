include(FetchContent)

set(CMSIS_CORE_VERSION v5.6.0_cm0)
FetchContent_Declare(cmsis_core
        URL https://github.com/STMicroelectronics/cmsis_core/archive/refs/tags/${CMSIS_CORE_VERSION}.zip)
FetchContent_MakeAvailable(cmsis_core)

set(CMSIS_DEVICE_F0_VERSION v2.3.6)
FetchContent_Declare(cmsis_device_f0
        URL https://github.com/STMicroelectronics/cmsis_device_f0/archive/refs/tags/${CMSIS_DEVICE_F0_VERSION}.zip)
FetchContent_MakeAvailable(cmsis_device_f0)

add_library(cmsis-core)
target_sources(cmsis-core PRIVATE
        ${cmsis_device_f0_SOURCE_DIR}/Source/Templates/gcc/startup_stm32f072xb.s
        system_stm32f0xx.c
        )
target_compile_definitions(cmsis-core PUBLIC STM32F072xB=1)
target_include_directories(cmsis-core PUBLIC
        ${cmsis_core_SOURCE_DIR}/Include
        ${cmsis_device_f0_SOURCE_DIR}/Include
        )