# README
## Prerequisites
All packages are for Ubuntu distribution.
```bash
$ sudo apt install build-essential clang-format cmake gcc gcc-arm-none-eabi gdb-multiarch lcov openocd
```
Download Renode and install it.
## Build project
A bash script is available to build and debug/run firmware or test suite.
The entrypoint for the script is to pass help option.
```bash
$ ./build.sh --help
```
You can build, clean or run on a MODE context (MODE=<debug, release or test>)
```bash
$ ./build.sh --build <MODE>
$ ./build.sh --run <MODE>
$ ./build.sh --clean <MODE>
```
## Open a debug session on board
Use build script to debug your program on the target :
```bash
$ ./build.sh --build debug
$ ./build.sh --run debug
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
