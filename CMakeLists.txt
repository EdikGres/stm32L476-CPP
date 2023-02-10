cmake_minimum_required(VERSION 3.16)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")


project(STM32L476 C CXX ASM)
set(TARGET stm32)

set(CROSS_TARGET_TRIPLET "arm-none-eabi-")
set(BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/)

# Fill in the target CPU here
set(CPU "cortex-m4")

# Fill in the linker script to use here
set(LINKER_SCRIPT "${BASE_DIR}/STM32L476VGTx_FLASH.ld")

set(STARTUP_SCRIPT ${CMAKE_SOURCE_DIR}/startup_stm32l476xx.s)

# Fill in the config for openocd to use.
# E.g. for an STM32F4-Discovery dev-board:
#set(OPENOCD_CFG "/usr/share/openocd/scripts/board/stm32f4discovery.cfg")

# We're not using add_definitions() here, since this would break the calls to the assembler.
# Some examples:
set(DEFINITIONS "${DEFINITIONS} -DSTM32L476xx")
set(DEFINITIONS "${DEFINITIONS} -DSEMIHOSTING")


# Prepend the cross-compiler prefix to the used tools.
set(CMAKE_C_COMPILER "${CROSS_TARGET_TRIPLET}gcc")
set(CMAKE_CXX_COMPILER "${CROSS_TARGET_TRIPLET}g++")
set(CMAKE_ASM_COMPILER "${CROSS_TARGET_TRIPLET}as")

# Be verbose when compiling.
set(WARNING_FLAGS "-Wall -Wextra -Wpedantic")

# Flags which are required for gcc, g++ and as.
set(COMMON_COMPILE_FLAGS "-mcpu=${CPU} -mthumb" )

# Flags for linker ld
#set(LINKER_FLAGS "--specs=nosys.specs -Wl,--print-memory-usage")
set(LINKER_FLAGS "-lc -l rdimon --specs=rdimon.specs -Wl,--print-memory-usage")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_COMPILE_FLAGS} ${WARNING_FLAGS} ${DEFINITIONS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} ${WARNING_FLAGS} ${DEFINITIONS} -std=c++14")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMMON_COMPILE_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS} -T ${LINKER_SCRIPT}")

# The following is required to get rid of the (not supported) -rdynamic flag
# when linking the final binary.
#set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "" )
#set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "" )

# Fill in your source files here.
#set(SOURCES
#    "${BASE_DIR}/drivers/CMSIS/system_stm32l4xx.c"
#	"${BASE_DIR}/src/main.cpp"
#)

file(GLOB_RECURSE SOURCES "src/*.*" "libs/*.*" "drivers/CMSIS/*.*" )

# Fill in your header files here. Not actually required for compiling, but
# some IDEs (e.g. QtCreator) won't list the header files in the project tree
# if they are not explicitly listed.
#set(HEADERS
	#"${BASE_DIR}/drivers/CMSIS/stm32l476xx.h"
	#)

# Fill in the directories of your header files here.
include_directories( drivers/CMSIS )

# Build the image {{{

# Compile the sources to an .elf
add_executable(${TARGET}.elf ${SOURCES} ${HEADERS} ${STARTUP_SCRIPT})

# Create a binary and hex from the elf
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CROSS_TARGET_TRIPLET}objcopy -Oihex ${TARGET}.elf ${TARGET}.hex
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    
    COMMAND ${CROSS_TARGET_TRIPLET}objcopy --input-target=ihex --output-target=binary ${TARGET}.hex ${TARGET}.bin
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Building ${TARGET}.bin and ${TARGET}.hex completed"
)


#}}}

# Targets for debugging/flashing {{{

# Start debugging with GDB (openocd needs to run in background)
add_custom_target(gdb
    COMMAND ${CROSS_TARGET_TRIPLET}gdb -x ${BASE_DIR}/gdb/gdb.cfg ${TARGET}.elf
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${TARGET}.elf
)

# Flash the previously built firmware to the target (openocd needs to run in background)
add_custom_target(flash
    COMMAND ${CROSS_TARGET_TRIPLET}gdb --batch -x ${BASE_DIR}/gdb/gdb.cfg -ex 'monitor program ${TARGET}.hex verify reset'
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${TARGET}.elf
)

# Flash the previously built firmware to the target (openocd needs to run in background)
add_custom_target(openocd
    COMMAND openocd -f ${OPENOCD_CFG}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${TARGET}.elf
)

#}}}

# vim: set ts=4 sw=4 sts=4 et smarttab :
