add_subdirectory(stm32f072rbtx)

add_library(stm32f072b-disco INTERFACE)
target_link_libraries(stm32f072b-disco INTERFACE stm32f072rbtx)
