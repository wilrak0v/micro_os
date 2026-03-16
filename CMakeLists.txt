cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(micro_os C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

add_executable(micro_os
    main.c
)

pico_enable_stdio_usb(micro_os 1)
pico_enable_stdio_uart(micro_os 0)

target_link_libraries(micro_os pico_stdlib pico_multicore)

# Ajoute LVGL
add_subdirectory(lvgl)
target_link_libraries(micro_os lvgl::lvgl)

target_include_directories(micro_os PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

pico_add_extra_outputs(micro_os)
