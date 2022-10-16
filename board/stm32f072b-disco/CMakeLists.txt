add_library(stm32f072b-disco INTERFACE)
set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/flash.ld)
# Use INTERFACE visible to public linker script to firmware executable target
target_link_options(stm32f072b-disco INTERFACE -T ${LINKER_SCRIPT})
set_target_properties(stm32f072b-disco PROPERTIES INTERFACE_LINK_DEPENDS ${LINKER_SCRIPT})

add_subdirectory(cmsis-core)
add_subdirectory(driver)
target_link_libraries(stm32f072b-disco INTERFACE driver)