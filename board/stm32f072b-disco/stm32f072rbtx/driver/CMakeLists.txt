include(FetchContent)

set(STM32F0XX_HAL_DRIVER_VERSION v1.7.6)
FetchContent_Declare(stm32f0xx_hal_driver DOWNLOAD_EXTRACT_TIMESTAMP
        URL https://github.com/STMicroelectronics/stm32f0xx_hal_driver/archive/refs/tags/${STM32F0XX_HAL_DRIVER_VERSION}.zip
        )
FetchContent_MakeAvailable(stm32f0xx_hal_driver)

aux_source_directory(${stm32f0xx_hal_driver_SOURCE_DIR}/Src SOURCES)
file(GLOB SOURCES "${stm32f0xx_hal_driver_SOURCE_DIR}/Src/*_ll_*.c")

add_library(driver)
target_sources(driver PRIVATE ${SOURCES})
target_include_directories(driver PUBLIC . ${stm32f0xx_hal_driver_SOURCE_DIR}/Inc)
target_link_libraries(driver PUBLIC cmsis-stm32f072xb)