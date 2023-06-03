cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-stm32f4xx.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OPENOCD_EXECUTABLE C:/devel/OpenOCD-20230202-0.12.0/bin/openocd.exe)

project(firmware)
set(TARGET_ELF ${PROJECT_NAME}.elf)

add_definitions(-DSTM32F429xx)
# add_definitions(-DUSE_HAL_DRIVER)
add_definitions(-DHSE_VALUE=8000000)
add_definitions(-DSTM32F429_439xx)
# add_definitions(STM32F429_439xx)

add_executable(${TARGET_ELF} src/main.cpp)
enable_language(C CXX ASM)
target_compile_features(${TARGET_ELF} PRIVATE cxx_std_17)

# add_compile_definitions(STM32F429_439xx)

#add_subdirectory(wch)
add_subdirectory(src)
add_subdirectory(bsp)
add_subdirectory(linker)
add_subdirectory(CherryUSB)

target_link_libraries(${TARGET_ELF} "m" "c" "nosys")

# add_custom_command(
#    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/romfs.c
#    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/wch/rtthread/tools/mkromfs.py ${RT_DFS_ROMFS_PATH} ${CMAKE_CURRENT_BINARY_DIR}/romfs.c)
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CROSS_OBJDUMP} "-CdSx" $<TARGET_FILE:${TARGET_ELF}> > $<TARGET_FILE:${TARGET_ELF}>.lst
    COMMENT "Generating disassembler file")
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CROSS_OBJCOPY} "-Obinary" $<TARGET_FILE:${TARGET_ELF}> $<TARGET_FILE:${TARGET_ELF}>.bin
    COMMENT "Generating bin file")
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CROSS_OBJCOPY} "-Oihex" $<TARGET_FILE:${TARGET_ELF}> $<TARGET_FILE:${TARGET_ELF}>.hex
    COMMENT "Generating hex file")
#add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
#    COMMAND ${CROSS_SIZE} "-A" $<TARGET_FILE:${TARGET_ELF}>
#    COMMENT "Output size of file")


add_custom_target(flash
        COMMAND ${OPENOCD_EXECUTABLE} [-f interface/stlink-v2.cgf,
                                        -f target/stm32f4x.cfg] 
            -c init
            -c halt
            -c "program ${TARGET_ELF} verify"
            -c wlink_reset_resume
            -c exit
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${tgt}
        COMMENT "Running openocd ${tgt_name}"
        USES_TERMINAL)
