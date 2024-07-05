# STM32_Framework_Template
## Nucleo-f446re board
## Prerequisites
All packages are for Ubuntu distribution.
```bash
$ sudo apt install build-essential clang-format cmake gcc gcc-arm-none-eabi gdb-multiarch lcov openocd
```
Download Renode and install it.
## Build project
### Build and flash embedded firmware (cross-compilation)
```bash
$ cmake -B build/<Debug or Release> -DCMAKE_BUILD_TYPE=<Debug or Release>
$ cmake --build build/<Debug or Release> # Build sources
$ cmake --build build/<Debug or Release> --target flash # Flash the firmware
$ cmake --build build/<Debug or Release> --target clean # Clean the build
```
### Build tests (native compilation)
```bash
$ cmake -B build/Test -DCMAKE_BUILD_TYPE=Test
$ cmake --build build/Test
$ ctest -V --test-dir build/Test # Run all tests with verbose output
$ cmake --build build/Test --target coverage # Generate code coverage report
```
## Open a debug session on board
Start gdb server with Openocd :
```bash
$ openocd -f config/openocd.cfg -c "setup stm32f4x" -c "program_debug"
```
Use gdb to debug your program :
```bash
$ gdb-multiarch --tui build/Debug/src/weather_sensors.elf
(gdb) target extended-remote localhost:3333
(gdb) load
(gdb) monitor reset halt
(gdb) continue
```
## Use the Renode emulator
You can use the emulator on standalone mode :
```bash
$ renode config/nucleo-f446re.resc
(machine-0) logFile @/tmp/function-trace.log
(machine-0) start
(machine-0) pause
(machine-0) sysbus.cpu PC
(machine-0) sysbus.cpu LogFunctionNames true
(machine-0) machine GetTimeSourceInfo
(machine-0) machine EnableProfiler
(machine-0) quit
```
Or with gdb :
```bash
$ gdb-multiarch --tui build/Debug/src/weather_sensors.elf
(gdb) target extended-remote localhost:3333
(gdb) monitor start # With renode only
(gdb) monitor halt # With renode only
(gdb) continue
```
## Acceptance Test
### Prerequisites
1. Get your python version on your distro:
```bash
$ python3 --version
Python 3.XX.YY
```
2. Install the python virtual package:
```bash
$ sudo apt install python3.XX-venv
```
3. Create a Python virtual environment:
```bash
$ python3 -m venv virtualenv
```
### Use virtual environment to development
1. Activate this:
```bash
$ source virtualenv/bin/activate
```
2. Install dependencies:
```bash
(virtualenv) pip install -r requirements.txt
```
3. And when you have finished to deactivate:
```bash
(virtualenv) deactivate
```
### Run test suite
Start emulator(or plug the board with firmware)
```bash
$ renode --disable-gui config/nucleo-f446re.resc &>/dev/null &
```
Launch python script:
```bash
(virtualenv) robot -d output TestShell.robot
```
