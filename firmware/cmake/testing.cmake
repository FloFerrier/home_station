include(CTest)

enable_testing()

macro(add_unit_test unitestname)
    message(NOTICE "Create test executable: ${ARGV0}")
    add_executable(${ARGV0})
    target_sources(${ARGV0} PRIVATE ${ARGN})
    target_include_directories(${ARGV0} PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/test/mock)
    target_compile_definitions(${ARGV0} PRIVATE -DTEST)
    target_compile_options(${ARGV0} PRIVATE -std=c99 -Og -g -Wall -W -Wextra)
    target_link_libraries(${ARGV0} PRIVATE cmocka-static bme68x)
    add_test(NAME ${ARGV0} COMMAND $<TARGET_FILE:${ARGV0}>)
endmacro()