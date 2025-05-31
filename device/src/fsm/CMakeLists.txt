add_library(fsm STATIC)

target_sources(fsm PRIVATE fsm.c)

target_link_libraries(fsm PRIVATE
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_freertos,freertos_kernel>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_console,console>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_led,led>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_sensor,sensor>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_shell,shell>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_stm32f4x,stm32_hal_driver>
)

target_include_directories(fsm PUBLIC .)