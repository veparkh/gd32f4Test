# Toolchain file for GD32F450VG (ARM Cortex-M4F) using arm-none-eabi-gcc

set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  arm)

# Don't try to link a full executable when testing the compiler
# (there is no host C runtime for a bare-metal target).
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# --- Toolchain executables ---------------------------------------------------
# arm-none-eabi-* must be on PATH (you already verified arm-none-eabi-gcc).
set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)

set(CMAKE_OBJCOPY       arm-none-eabi-objcopy CACHE INTERNAL "")
set(CMAKE_SIZE          arm-none-eabi-size    CACHE INTERNAL "")

# --- CPU / FPU flags ---------------------------------------------------------
# Cortex-M4 with single-precision hardware FPU. These MUST be identical for
# C, ASM and the linker, otherwise you get ABI mismatch errors at link time.
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS_INIT   "${CPU_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_ASM_FLAGS_INIT "${CPU_FLAGS} -x assembler-with-cpp")
set(CMAKE_CXX_FLAGS_INIT "${CPU_FLAGS} -ffunction-sections -fdata-sections")

# Linker: garbage-collect unused sections, use nano newlib, print map file.
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CPU_FLAGS} -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs")

# Search for programs on the host, but headers/libs only in the target sysroot.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
