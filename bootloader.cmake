
add_executable(bootloader 
    bootloader.c
)



# pull in common dependencies
target_link_libraries(bootloader 
    pico_stdlib
    #pico_multicore
    

    #hardware_flash
    hardware_irq
    hardware_sync
    #hardware_spi
    #hardware_i2c
    #hardware_timer
    #hardware_clocks
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(bootloader pico_cyw43_arch_none)
endif()




#######




# enable usb output, disable uart output
pico_enable_stdio_usb(bootloader 1)
pico_enable_stdio_uart(bootloader 0)
# add url via pico_set_program_url

# Força rodar em SRAM
#pico_set_binary_type(bootloader copy_to_ram)



# create map/bin/hex file etc.
pico_add_extra_outputs(bootloader)
