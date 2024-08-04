#!/bin/bash

BUILD_MODE=("debug" "release" "test")

function display_help() {
    echo "Usage: $(basename $0) [-h] [-b|c|r MODE]"
    echo "Options:"
    echo "  -h, --help     Display this help message"
    echo "  -b, --build    Configure workspace and build sources for selected MODE"
    echo "  -c, --clean    Remove workspace fir selected MODE"
    echo "  -r, --run      Run the selected MODE"
}

# 0 => False
# 1 => True
function check_build_mode_is_available() {
    local arg=$1
    for mode in "${BUILD_MODE[@]}"; do
        if [ "${arg}" = "${mode}" ]; then
            return 1
        fi
    done
    return 0
}

function execute_build() {
    local arg_build_mode=$1
    check_build_mode_is_available ${arg_build_mode}
    if [ $? -eq 1 ]; then
        echo "Build on ${arg_build_mode} mode"
        cmake -B build/${arg_build_mode} -DCMAKE_BUILD_TYPE=${arg_build_mode}
        cmake --build build/${arg_build_mode}
    else
        echo "Selected build mode is incorrect ..."
        echo "MODE=[${BUILD_MODE[@]}]"
    fi
}

function execute_clean() {
    local arg_build_mode=$1
    check_build_mode_is_available ${arg_build_mode}
    if [ $? -eq 1 ]; then
        folder="build/${arg_build_mode}"
        echo "Clean ${folder} folder"
        rm -rf ${folder}
    else
        echo "Selected build mode is incorrect ..."
        echo "MODE=[${BUILD_MODE[@]}]"
    fi
}

function flash_firmware() {
    local filename=$1
    if [ -f "$filename" ]; then
        openocd -f config/openocd.cfg -c "setup stm32f4x" -c "program_release ${filename}"
    else 
        echo "$filename does not exist."
        echo "Launch the build command before the run."
    fi
}

function run_test_suite() {
    local folder=$1
    if [ -d "$folder" ]; then
        ctest -V --test-dir ${folder}
    else 
        echo "$folder does not exist."
        echo "Launch the build command before the run."
    fi
}

function run_debug_session() {
    local filename=$1
    if [ -f "$filename" ]; then
        openocd -f config/openocd.cfg -c "setup stm32f4x" -c "program_debug ${filename}" &>/dev/null &
        gdb-multiarch --tui ${filename}
    else
        echo "$filename does not exist."
        echo "Launch the build command before the run."
    fi
}

function execute_run() {
    local arg_mode=$1
    case ${arg_mode} in
        "release")
            echo "Release mode"
            flash_firmware "build/release/src/weather_sensors.elf"
            ;;

        "debug")
            echo "Debug mode"
            run_debug_session "build/debug/src/weather_sensors.elf"
            ;;

        "test")
            echo "Test mode"
            run_test_suite "build/test"
            ;;
        *)
            echo "??? mode"
            ;;
    esac
}

OPTS=$(getopt --options b:c:r:h --longoptions "build:,clean:,run:,help"  -n "parse-arg" -- $@)
while [ $# -gt 0 ]; do
    case "$1" in
        -h | --help)
            display_help
            exit 0
            ;;
        -b | --build)
            execute_build $2
            break
            ;;
        -c | --clean)
            execute_clean $2
            break
            ;;
        -r | --run)
            execute_run $2
            break
            ;;
        *)
            display_help
            break
            ;;
    esac
done