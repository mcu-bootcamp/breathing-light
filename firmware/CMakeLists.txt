if (CMAKE_CROSSCOMPILING)
    add_executable(firmware)
    target_sources(firmware PRIVATE main.cpp)
    target_link_libraries(firmware PRIVATE stm32f072b-disco)
    target_link_options(firmware PRIVATE LINKER:--print-memory-usage,-Map=$<TARGET_FILE:firmware>.map)

    add_custom_target(dist)
    add_dependencies(dist firmware)
    add_custom_command(TARGET dist POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:firmware> $<TARGET_FILE:firmware>.hex
            COMMAND ${CMAKE_COMMAND} -E echo "Exporting $<TARGET_FILE:firmware>.hex"
            )
    add_custom_command(TARGET dist POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:firmware> $<TARGET_FILE:firmware>.bin
            COMMAND ${CMAKE_COMMAND} -E echo "Exporting $<TARGET_FILE:firmware>.bin"
            )
endif ()
