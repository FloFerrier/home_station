# README
## Prerequisites
Todo: Should be cross-platform.
## Configure and build cross-compiling firmware
### Configure workspace
```bash
$ cmake -GNinja -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain-arm-none-eabi.cmake" -DCMAKE_BUILD_TYPE=debug -Bbuild/debug
```
Note:
- **CMAKE_BUILD_TYPE** can be debug or release.
- **CMAKE_TOOLCHAIN_FILE** can be replace by **--toolchain**
- Add **--fresh** to configure with new tree folder
### Build source
```bash
$ cmake --build build/debug
```
### Flash firmware
Todo
## Configure and build native test suite
### Configure workspace
```bash
$ cmake -GNinja -DCMAKE_BUILD_TYPE=test -Bbuild/test
```
Note:
- Add **--fresh** to configure with new tree folder
- **CMAKE_TOOLCHAIN_FILE** is not used here because CMake find native toolchain automatically.
### Build source
```bash
$ cmake --build build/test
```
### Run test suite
```bash
$ ctest -V --test-dir build/test
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
