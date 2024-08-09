#include "commands.h"
#include "console.h"

#include "sensor.h"
#include "bme68x.h"

#include <stddef.h>
#include <string.h>

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif // TEST

#define COMMAND_STRING_LEN_MAX (255u)

typedef struct {
  const char *name;
  void (*handler)(void);
  const char *desc;
} command_s;

STATIC void command_unknown(void);
STATIC void command_help(void);
STATIC void command_sensorSelfTest(void);
STATIC void command_sensorGetData(void);

STATIC const command_s command_list[] = {
    [COMMAND_UNKNOWN] {"", command_unknown, ""},
    [COMMAND_HELP] {"help", command_help, "Display all commands available"},
    [COMMAND_SENSOR_SELF_TEST]{"sensor_selfTest", command_sensorSelfTest, "Performing a sensor self-test"},
    [COMMAND_SENSOR_GET_DATA]{"sensor_getData", command_sensorGetData, "Request a sensor to get data"},
};

STATIC void command_unknown(void) {
    console_send("{\"code\":\"FAILURE\", \"message\":\"Command unknown\", \"response\":\"Tap help to display all available command.\"}\r\n");
}

STATIC void command_help(void) {
    console_send("{\"code\":\"SUCCESS\", \"message\":\"\", \"response\":\"%s: %s, %s: %s\"}\r\n", command_list[COMMAND_SENSOR_SELF_TEST].name, command_list[COMMAND_SENSOR_SELF_TEST].desc, command_list[COMMAND_SENSOR_GET_DATA].name, command_list[COMMAND_SENSOR_GET_DATA].desc);
}

STATIC void command_sensorSelfTest(void) {
    int8_t result = sensor_selfTest();
    if(result == SENSOR_OK) {
        console_send("{\"code\":\"SUCCESS\", \"message\":\"%s\", \"response\":\"\"}\r\n", sensor_returnCodeAsString(result));
    }
    else {
        console_send("{\"code\":\"FAILURE\", \"message\":\"%s\", \"response\":\"\"}\r\n", sensor_returnCodeAsString(result));
    }
}

STATIC void command_sensorGetData(void) {
    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t result = sensor_getData(data, &number_of_data);
    if(result == SENSOR_OK) {
        console_send("{\"code\":\"SUCCESS\", \"message\":\"%s\", \"response\":\"%.1f deg, %.1f hPa %.1f rH\"}\r\n", sensor_returnCodeAsString(result), data[0].temperature_in_deg, data[0].pressure_in_pascal / 100.0f, data[0].humidity_in_per100);
    }
    else {
        console_send("{\"code\":\"FAILURE\", \"message\":\"%s\", \"response\":\"\"}\r\n", sensor_returnCodeAsString(result));
    }
}

command_index_e command_getIndex(const char *cmd) {
    if(cmd == NULL) {
        return COMMAND_UNKNOWN;
    }
    for(uint32_t index=0; index < sizeof(command_list)/sizeof(command_s); index++) {
        uint32_t cmp = strncmp(cmd, command_list[index].name, COMMAND_STRING_LEN_MAX);
        if(cmp == 0) {
            return index;
        }
    }
    return COMMAND_UNKNOWN;
}

bool command_execute(command_index_e command_index) {
    if((command_index >= COMMAND_UNKNOWN) && (command_index <= COMMAND_SENSOR_GET_DATA)) {
        command_list[command_index].handler();
        return true;
    }
    return false;
}
