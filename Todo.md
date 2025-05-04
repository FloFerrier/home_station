# TODO
## In Course
- [device] - Maintenance - Get task informations by shell command
- [device] - Feature - Update protocol to manage metadata

## Backlog
- [gateway] - Feature - Get data through Serial
- [gateway] - Feature - Store data from device on a database
- [device] - Documentation - Add prerequisites tools on Readme for cross-platform environment
- [device] - Maintenance - Add log to a file system like syslog
- [device] - Feature - Add RTC support to have timestamp
- [device] - Robustness - Add data sampling with a sensor task
- [device] - Robustness - Add watchdog if system add a problem and log it on filesystem
- [device] - Robustness - Add stack overflow checking with FreeRTOS
- [device] - Test - Portage to Ceedling/Unity/CMock test framework
- [device] - Quality - Add test coverage for unit test
- [device] - Quality - Add qualimetry metries
- [device] - Quality - Add cppcheck misra checking
```bash
# Command exemple
$ cppcheck --enable=all --addon=misra -I src/commands/ -I src/console/ -I src/fsm -I src/led -I src/protocol -I src/sensor -I src/shell src/commands/commands.c
```
- [device] - Build system - Add automatic format with clang-format on CMake
- [device] - Build system - Add emulator support on CMake with Renode
- [system] - Architecture - Documenting the design (Application/Middleware/Low-level/Third-party)
- [system] - Feature - Update protocol format with the following schema :
```bash
{
	"request": {
		"code": string,
		"message": string,
	},
	"data": [
		{
			"field": string,
			"value": float,
			"unit": string,
		}
	]
	"metadata": [
		{
			"field": string,
			"value": string,
		}
	]
}
```
- [system] - Feature - Add support for multi-data on protocol (Temperature, Pressure, Humidity)