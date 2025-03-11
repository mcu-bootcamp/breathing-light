include(FetchContent)

set(CMSIS_CORE_VERSION v5.6.0_cm0)
FetchContent_Declare(cmsis_core
        DOWNLOAD_EXTRACT_TIMESTAMP
        URL https://github.com/STMicroelectronics/cmsis-core/archive/refs/tags/${CMSIS_CORE_VERSION}.zip)
FetchContent_MakeAvailable(cmsis_core)
add_library(cmsis-core INTERFACE)
target_include_directories(cmsis-core INTERFACE ${cmsis_core_SOURCE_DIR}/Include)

set(CMSIS_DEVICE_F0_VERSION v2.3.7)
FetchContent_Declare(cmsis_device_f0
        DOWNLOAD_EXTRACT_TIMESTAMP
        URL https://github.com/canmor/cmsis-device-f0/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(cmsis_device_f0)

add_library(cmsis-device-f0 INTERFACE)
target_include_directories(cmsis-device-f0 INTERFACE ${cmsis_device_f0_SOURCE_DIR}/Include)
target_link_libraries(cmsis-device-f0 INTERFACE cmsis-core)

file(GLOB STARTUP_SOURCES "${cmsis_device_f0_SOURCE_DIR}/Source/Templates/gcc/startup_*.s")
foreach(STARTUP_SOURCE ${STARTUP_SOURCES})
    get_filename_component(STARTUP_NAME ${STARTUP_SOURCE} NAME_WE)
    string(REPLACE "startup_" "" DEVICE_NAME ${STARTUP_NAME})
    set(TARGET cmsis-${DEVICE_NAME})
    add_library(${TARGET})
    target_sources(${TARGET} PRIVATE
        ${STARTUP_SOURCE}
        ${cmsis_device_f0_SOURCE_DIR}/Source/Templates/system_stm32f0xx.c)
    target_link_libraries(${TARGET} PUBLIC cmsis-device-f0)
    string(TOUPPER ${DEVICE_NAME} DEVICE_NAME_UPPER)
    string(REGEX REPLACE "X$" "x" DEVICE_NAME_UPPER ${DEVICE_NAME_UPPER})
    string(REGEX REPLACE "X(.)$" "x\\1" DEVICE_NAME_UPPER ${DEVICE_NAME_UPPER})
    target_compile_definitions(${TARGET} PUBLIC ${DEVICE_NAME_UPPER}=1)
endforeach()