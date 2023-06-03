target_sources(${TARGET_ELF} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32f429.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system_stm32f4xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOSHooks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos.cpp
)

target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
