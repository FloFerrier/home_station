enable_testing()

setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE ctest
    EXCLUDE ${CMAKE_SOURCE_DIR}/test/*
)

macro(add_unit_test unitestname)
    add_executable(${ARGV0} ${ARGN})
    target_include_directories(${ARGV0} PRIVATE ${CMAKE_SOURCE_DIR}/src mock ${CMAKE_SOURCE_DIR}/lib/driver/bme68x/src)
    target_compile_definitions(${ARGV0} PRIVATE -DTEST)
    target_compile_options(${ARGV0} PRIVATE -std=c99 -Og -g -Wall -W -Wextra --coverage)
    target_link_libraries(${ARGV0} PRIVATE cmocka-static bme68x)
    target_link_options(${ARGV0} PRIVATE --coverage)
    add_test(NAME ${ARGV0} COMMAND $<TARGET_FILE:${ARGV0}>)
    append_coverage_compiler_flags_to_target(${ARGV0})
endmacro()

add_unit_test(test_shell
    ${CMAKE_SOURCE_DIR}/src/shell.c
    mock/mock_console.c
    mock/mock_commands.c
    ut/test_shell.c
)

add_unit_test(test_bme680
    ut/test_bme680.c
)

add_unit_test(test_sensor
${CMAKE_SOURCE_DIR}/src/sensor.c
    mock/mock_bme68x.c
    mock/mock_console.c
    ut/test_sensor.c
)
