target_sources(${TARGET_ELF} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/class/cdc/usbd_cdc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/core/usbd_core.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/osal/usb_osal_freertos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/port/dwc2/usb_dc_dwc2.c
)

target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/class/cdc)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common)
# target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/osal)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/port/dwc2)
