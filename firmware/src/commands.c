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
    console_send("{\"code\":400, \"message\":\"Bad request\", \"response\":\"Tap help to display all available command.\"}\r\n");
}

STATIC void command_help(void) {
    console_send("{\"code\":200, \"message\":\"Success\", \"response\":\"\"}\r\n");
}

STATIC void command_sensorSelfTest(void) {
    int8_t result = sensor_selfTest();
    if(result == BME68X_OK) {
        console_send("(sensor)> Self-test passed !\r\n");
    }
    else {
        console_send("{\"code\":503, \"message\":\"Service Unavailable\", \"response\":\"\"}\r\n");
    }
}

STATIC void command_sensorGetData(void) {
    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t result = sensor_getData(data, &number_of_data);
    if (result != BME68X_OK) {
        console_send("{\"code\":503, \"message\":\"Service Unavailable\", \"response\":\"\"}\r\n");
    }
    else {
        console_send("(sensor)> Number of data available %d\r\n", number_of_data);
        for(uint32_t index=0; index<number_of_data; index++) {
            console_send("(sensor)> Temperature %.1f deg, Pressure %.1f hPa, Humidity %.1f %rH\r\n",
                data[index].temperature_in_deg, data[index].pressure_in_pascal / 100.0f, data[index].humidity_in_per100);
        }
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
