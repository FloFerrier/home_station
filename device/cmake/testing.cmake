include(CTest)

enable_testing()

macro(add_unit_test unitestname)
    message(NOTICE "Create test executable: ${ARGV0}")
    add_executable(${ARGV0})
    target_include_directories(${ARGV0} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/console
        ${CMAKE_SOURCE_DIR}/src/commands
        ${CMAKE_SOURCE_DIR}/src/sensor
        ${CMAKE_SOURCE_DIR}/src/shell
        ${CMAKE_SOURCE_DIR}/src/led
        ${CMAKE_SOURCE_DIR}/src/fsm
        ${CMAKE_SOURCE_DIR}/src/protocol
        ${CMAKE_SOURCE_DIR}/test/mock
    )
    target_sources(${ARGV0} PRIVATE ${ARGN})
    target_compile_definitions(${ARGV0} PRIVATE -DTEST)
    target_compile_options(${ARGV0} PRIVATE -std=c99 -Og -g -Wall -W -Wextra -Werror)
    target_link_libraries(${ARGV0} PRIVATE cmocka-static bme68x)
    add_test(NAME ${ARGV0} COMMAND $<TARGET_FILE:${ARGV0}>)
endmacro()