add_library(stm32f072rbtx INTERFACE)
target_linker_script(stm32f072rbtx ${CMAKE_CURRENT_LIST_DIR}/flash.ld)

add_subdirectory(cmsis-core)
target_link_libraries(stm32f072rbtx INTERFACE cmsis-stm32f072xb)
