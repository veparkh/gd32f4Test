cmake_minimum_required(VERSION 3.20)

project(GD32f4Test C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_definitions(GD32F450 HXTAL_VALUE=8000000)

set(TARGET ${PROJECT_NAME})
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Startup/gd32f450xG_flash.ld)

set(FREERTOS_DIR ${CMAKE_SOURCE_DIR}/FreeRTOS)

# --- Include paths -----------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/Core
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/Include
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Include
    ${FREERTOS_DIR}
    ${FREERTOS_DIR}/Source/include
    ${FREERTOS_DIR}/Source/portable/GCC/ARM_CM4F
)

# --- Peripheral drivers ------------------------------------------------------
set(PERIPH_SRC
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_rcu.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_gpio.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_adc.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_can.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_dma.c
    ${CMAKE_SOURCE_DIR}/Drivers/GD32F4xx_Periph/Source/gd32f4xx_misc.c
)

# --- FreeRTOS ----------------------------------------------------------------
set(FREERTOS_SRC
    ${FREERTOS_DIR}/Source/tasks.c
    ${FREERTOS_DIR}/Source/queue.c
    ${FREERTOS_DIR}/Source/list.c
    ${FREERTOS_DIR}/Source/timers.c
    ${FREERTOS_DIR}/Source/event_groups.c
    ${FREERTOS_DIR}/Source/portable/GCC/ARM_CM4F/port.c
    ${FREERTOS_DIR}/Source/portable/MemMang/heap_4.c
)

# --- Core --------------------------------------------------------------------
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
    ${FREERTOS_SRC}
    ${STARTUP_SRC}
)

target_link_options(${TARGET}.elf PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${TARGET}.map
    -Wl,--print-memory-usage
)

add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${TARGET}.elf> ${TARGET}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}.elf> ${TARGET}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET}.elf>
    COMMENT "Generating .hex / .bin and printing size"
)