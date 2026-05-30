cmake_minimum_required(VERSION 3.20)

# Language must be enabled AFTER the toolchain is set. We pass the toolchain
# file on the command line (see build command below).
project(GD32f4Test C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Target chip define required by gd32f4xx.h to select the right register map.
add_compile_definitions(GD32F450)

set(TARGET ${PROJECT_NAME})
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Startup/gd32f450xG_flash.ld)

# --- Include paths -----------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/Core
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/Include
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Include
)

# --- Source files ------------------------------------------------------------
# Peripheral drivers: only the ones we actually need for this task.
set(PERIPH_SRC
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_rcu.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_gpio.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_adc.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_can.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_dma.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_misc.c
)

set(CORE_SRC
    ${CMAKE_SOURCE_DIR}/Core/main.c
    ${CMAKE_SOURCE_DIR}/Core/system_gd32f4xx.c
    ${CMAKE_SOURCE_DIR}/Core/gd32f4xx_it.c
    ${CMAKE_SOURCE_DIR}/Core/systick.c
    ${CMAKE_SOURCE_DIR}/Core/syscalls.c
)

set(STARTUP_SRC
    ${CMAKE_SOURCE_DIR}/Startup/startup_gd32f450_470.S
)

add_executable(${TARGET}.elf
    ${CORE_SRC}
    ${PERIPH_SRC}
    ${STARTUP_SRC}
)

# --- Linking -----------------------------------------------------------------
target_link_options(${TARGET}.elf PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${TARGET}.map
    -Wl,--print-memory-usage
)

# --- Post-build: .hex, .bin, size --------------------------------------------
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${TARGET}.elf> ${TARGET}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}.elf> ${TARGET}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET}.elf>
    COMMENT "Generating .hex / .bin and printing size"
)
