# HOME STATION

[![Device](https://github.com/FloFerrier/home_station/actions/workflows/device.yml/badge.svg?branch=main)](https://github.com/FloFerrier/home_station/actions/workflows/device.yml)

The home station is a hobbist project and in no way an industrial project. The goal of this project is to have fun with state of art on embedded system.

## Prerequisites
Todo: Should be cross-platform.
- gcc-arm-none-eabi
- cmake
- openocd
- renode
- picocom
- doxygen
- lcov/gcov
- make

Third-parties are managed by git-submodule, so you need to get it :
```bash
$ git submodule update --init --recursive
```
## Build system
A Makefile is available to manage build system, tap the following command to have all command with description :
```bash
$ make help
```

## Open a debug session on target board
After steps configure workspace and build source as debug mode, it is possible to launch a session debug with this command :
```bash
$ cmake --build build/debug --target debug
```
On another terminal, launch gdb :
```bash
$ arm-none-eabi-gdb --tui
(gdb) target extended-remote localhost:3333
(gdb) file <FIRMWARE_FILE>
(gdb) load
(gdb) monitor reset halt
(gdb) continue
...
(gdb) monitor shutdown
(gdb) exit
```
Note:
- Use **gdb-multiarch** instead of **arm-none-eabi-gdb** if you have trouble with the latter.
- On Linux, if you do not shutdown openocd from gdb, you have some errors like : "Error: couldn't bind tcl to socket on port 6666: Address already in use"
So, you can stop it with this command :
```bash
$ kill -9 `lsof -i -a -c openocd`
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
$ arm-none-eabi-gdb --tui
(gdb) target extended-remote localhost:3333
(gdb) file <FIRMWARE_FILE>
(gdb) load
(gdb) monitor start # With renode only
(gdb) monitor halt # With renode only
(gdb) continue
```

## License
Check the License file to have terms.