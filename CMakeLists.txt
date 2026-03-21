cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(micro_os C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico2)

pico_sdk_init()

add_executable(micro_os
    src/main.c
    src/drivers/spi_dma.c
    src/drivers/screen.c
    src/micro_lib/micro_lib.c
    src/micro_term/micro_term.c
)

pico_enable_stdio_usb(micro_os 1)
pico_enable_stdio_uart(micro_os 0)

# Ajoute LVGL
set(LV_CONF_BUILD_DISABLE_DEMO_BENCHMARK 0)
add_subdirectory(lvgl)
target_link_libraries(micro_os PRIVATE
    pico_stdlib
    pico_multicore
    hardware_spi
    hardware_dma
    hardware_irq
    lvgl::lvgl
)

target_include_directories(micro_os PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    src/
    src/drivers
    src/micro_lib
    src/micro_term
)

pico_add_extra_outputs(micro_os)
