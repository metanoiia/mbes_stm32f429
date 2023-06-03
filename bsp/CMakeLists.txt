aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/STM32F4_HAL/Src STMHAL_SOURCES)

target_sources(${TARGET_ELF} PUBLIC
    ${STMHAL_SOURCES}
)


target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include) 
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/STM32F4xx/Include)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/STM32F4_HAL/Include)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/STM32F4_HAL/Include/Legacy)

add_subdirectory(FreeRTOS)