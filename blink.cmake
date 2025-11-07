# Generated Cmake Pico project file

# Add executable. Default name is the project name, version 0.1

add_executable(blink
    menu.c  # <-- Arquivo-fonte ATUALIZADO
    )


#pico_set_linker_script(blink ${CMAKE_CURRENT_SOURCE_DIR}/blink_memory.ld)

set_target_properties(blink PROPERTIES
    LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/blink_memory.ld"
    LINK_FLAGS "-T${CMAKE_CURRENT_SOURCE_DIR}/blink_memory.ld"
)

# pull in common dependencies
target_link_libraries(blink 
    pico_stdlib
    hardware_watchdog # Adicionado para suportar a função 'watchdog_reboot'
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(blink pico_cyw43_arch_none)
endif()


# enable usb output, disable uart output
pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)


# create map/bin/hex file etc.
pico_add_extra_outputs(blink)