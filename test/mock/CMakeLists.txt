set(MOCK_LIST
    mock_bme68x
    mock_commands
    mock_console
    mock_freertos
    mock_led
    mock_protocol
    mock_sensor
    mock_shell
    mock_stm32f4x
)

foreach(MOCK_ITEM IN LISTS MOCK_LIST)
    message("${MOCK_ITEM}")
    add_library(${MOCK_ITEM} STATIC)
    target_sources(${MOCK_ITEM} PRIVATE ${MOCK_ITEM}.c)
    target_link_libraries(${MOCK_ITEM} PRIVATE cmocka)
    target_include_directories(${MOCK_ITEM} PUBLIC
        ${CMAKE_SOURCE_DIR}/test/mock
        ${CMAKE_SOURCE_DIR}/src/console
        ${CMAKE_SOURCE_DIR}/src/commands
        ${CMAKE_SOURCE_DIR}/src/sensor
        ${CMAKE_SOURCE_DIR}/src/shell
        ${CMAKE_SOURCE_DIR}/src/led
        ${CMAKE_SOURCE_DIR}/src/fsm
        ${CMAKE_SOURCE_DIR}/src/protocol
        ${CMAKE_SOURCE_DIR}/third_party/driver/bme68x/src
    )
endforeach()