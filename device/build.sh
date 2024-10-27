#!/bin/bash

BUILD_MODE=("debug" "release" "test")
TARGET_MODE=("emulator" "device")

function display_help() {
    echo "Usage: $(basename $0) [-h] [-b|c|r MODE] [-t TARGET]"
    echo "Options:"
    echo "  -h, --help              Display this help message"
    echo "  -b, --build             Configure workspace and build sources for selected MODE"
    echo "  -c, --clean             Remove workspace for selected MODE"
    echo "  -r, --run               Run the selected MODE"
    echo "  -t, --acceptance-test   Launch acceptance test on the TARGET"
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
            flash_firmware "build/release/src/device_firmware.elf"
            ;;

        "debug")
            run_debug_session "build/debug/src/device_firmware.elf"
            ;;

        "test")
            run_test_suite "build/test"
            ;;
        *)
            echo "??? mode"
            ;;
    esac
}

function run_acceptance_test_on_emulator() {
    echo "Execute acceptance test on emulator"
    echo "Launch renode as emulator"
    renode --disable-gui config/nucleo-f446re.resc &> test/acceptance/output/renode.log &
    echo "Wait emulator is running ..."
    sleep 2
    cd test/acceptance
    VENV=virtualenv
    PYTHON=${VENV}/bin/python
    PIP=${VENV}/bin/pip
    python3 -m venv ${VENV}
    ${PIP} install -r requirements.txt
    source ${VENV}/bin/activate
    robot --outputdir output --variable SERIAL_PORT:/tmp/console TestShell.robot
    deactivate
    kill %1
    rm -rf /tmp/console
    echo "Emulator stopped."
    echo "Check emulator log on test/acceptance/renode.log"
}

function run_acceptance_test_on_device() {
    echo "Execute acceptance test on device"
    flash_firmware "build/debug/src/device_firmware.elf"
    echo "Wait firmware on debug mode is flashing ..."
    sleep 2
    cd test/acceptance
    VENV=virtualenv
    PYTHON=${VENV}/bin/python
    PIP=${VENV}/bin/pip
    python3 -m venv ${VENV}
    ${PIP} install -r requirements.txt
    source ${VENV}/bin/activate
    robot --outputdir output --variable SERIAL_PORT:/dev/ttyACM0 TestShell.robot
    deactivate
}

function execute_acceptance_test() {
    local arg_target_mode=$1
    case ${arg_target_mode} in
    "emulator")
        run_acceptance_test_on_emulator
        ;;

    "device")
        run_acceptance_test_on_device
        ;;

    *)
        echo "Selected target is incorrect ..."
        echo "TARGET=[${TARGET_MODE[@]}]"
        ;;
    esac
}

OPTS=$(getopt --options b:c:r:t:h --longoptions "build:,clean:,run:,acceptance-test:,help"  -n "parse-arg" -- $@)
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
        -t | --acceptance-test)
            execute_acceptance_test $2
            break
            ;;
        *)
            display_help
            break
            ;;
    esac
done