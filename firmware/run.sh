#!/bin/bash

execute_run() {
    run_mode=$1
    case ${run_mode} in
        "Flash")
            echo "Flash mode"
            openocd -f config/openocd.cfg -c "setup stm32f4x" -c "program_release build/Release/src/weather_sensors.elf"
            ;;

        "Debug")
            echo "Debug mode"
            openocd -f config/openocd.cfg -c "setup stm32f4x" -c "program_debug build/Debug/src/weather_sensors.elf" &>/dev/null &
            gdb-multiarch --tui build/Debug/src/weather_sensors.elf
            ;;

        "UnitTest")
            echo "Unit Test mode"
            ctest -V --test-dir build/Test
            ;;

        "TestBench")
            echo "Test Bench mode"
            python3 -m venv test/acceptance/virtualenv
            source test/acceptance/virtualenv/bin/activate
            pip install -r test/acceptance/requirements.txt
            robot -d test/acceptance/output test/acceptance/TestShell.robot
            deactivate
            ;;

        *)
            echo "??? mode"
            ;;
        esac
}

run_mode=("Debug" "Flash" "UnitTest" "TestBench")

parameter_mode=$1

for item in "${run_mode[@]}"; do
    if [ "$item" = "$parameter_mode" ]; then
        execute_run $parameter_mode
        break
    fi
done