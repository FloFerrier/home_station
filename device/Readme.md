# README
## Prerequisites
Todo: Should be cross-platform.

Third-parties are managed by git-submodule, so you need to get it :
```bash
$ git submodule update --init --recursive
```
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
```bash
$ cmake --build build/debug --target flash
```
## Configure and build native test suite
### Configure workspace
```bash
$ cmake -GNinja -DCMAKE_BUILD_TYPE=test -Bbuild/test
```
Note:
- Use **-G"Visual Studio 17 2022"** as generator on Windows
- Add **--fresh** to configure with new tree folder
- **CMAKE_TOOLCHAIN_FILE** is not used here because CMake find native toolchain automatically.
### Build source
```bash
$ cmake --build build/test
```
### Run test suite
For running all test binaries, launch this command :
```bash
$ ctest --test-dir build/test
```
For displaying all test binaries, launch this command :
```bash
$ ctest --test-dir build/test -N
```
For running only a specific test binary, launch this command :
```bash
$ ctest --test-dir build/test -R <test_name>
```
Note:
- Add **-C Debug** on Windows
- Add **-V** to have a verbose mode or **-VV** to have an extra verbose mode
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

## Shell on serial port com
```bash
$ picocom --echo -b 115200 /dev/ttyACM0
```
