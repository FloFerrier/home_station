#include "commands.h"

#include "sensor.h"
#include "console.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#ifndef TEST
#define STATIC static
#include <stm32f4xx_hal.h>
#else
#include "mock_stm32f4x.h"
#define STATIC
#endif // TEST

#define COMMAND_STRING_LEN_MAX (255u)

typedef struct {
  const char *name;
  void (*handler)(char *response, const uint32_t response_len_max);
  const char *desc;
} command_s;

STATIC void command_unknown(char *response, const uint32_t response_len_max);
STATIC void command_help(char *response, const uint32_t response_len_max);
STATIC void command_reboot(char *response, const uint32_t response_len_max);
STATIC void command_sensorSelfTest(char *response, const uint32_t response_len_max);
STATIC void command_sensorGetData(char *response, const uint32_t response_len_max);

STATIC const command_s command_list[] = {
    [COMMAND_UNKNOWN] {"", command_unknown, ""},
    [COMMAND_HELP] {"help", command_help, "Display all commands available"},
    [COMMAND_REBOOT] {"reboot", command_reboot, "Performing a system reboot"},
    [COMMAND_SENSOR_SELF_TEST]{"sensor_selfTest", command_sensorSelfTest, "Performing a sensor self-test"},
    [COMMAND_SENSOR_GET_DATA]{"sensor_getData", command_sensorGetData, "Request a sensor to get data"},
};

STATIC void command_unknown(char *response, const uint32_t response_len_max) {
    (void)snprintf(response, response_len_max, "{\"code\":\"FAILURE\", \"message\":\"Command unknown\", \"response\":\"Tap help to display all available command.\"}\r\n");
}

STATIC void command_help(char *response, const uint32_t response_len_max) {
    (void)snprintf(response, response_len_max, "{\"code\":\"SUCCESS\", \"message\":\"\", \"response\":\"%s: %s, %s: %s\"}\r\n", command_list[COMMAND_SENSOR_SELF_TEST].name, command_list[COMMAND_SENSOR_SELF_TEST].desc, command_list[COMMAND_SENSOR_GET_DATA].name, command_list[COMMAND_SENSOR_GET_DATA].desc);
}

STATIC void command_reboot(char *response, const uint32_t response_len_max) {
    (void)response;
    (void)response_len_max;
    /* Response must be done directly by console due to reset */
    console_send("{\"code\":\"SUCCESS\", \"message\":\"\", \"response\":\"\"}\r\n");
    NVIC_SystemReset();
}

STATIC void command_sensorSelfTest(char *response, const uint32_t response_len_max) {
    int8_t result = sensor_selfTest();
    if(result == SENSOR_OK) {
        (void)snprintf(response, response_len_max, "{\"code\":\"SUCCESS\", \"message\":\"%s\", \"response\":\"\"}\r\n", sensor_returnCodeAsString(result));
    }
    else {
        (void)snprintf(response, response_len_max, "{\"code\":\"FAILURE\", \"message\":\"%s\", \"response\":\"\"}\r\n", sensor_returnCodeAsString(result));
    }
}

STATIC void command_sensorGetData(char *response, const uint32_t response_len_max) {
    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t result = sensor_getData(data, &number_of_data);
    if(result == SENSOR_OK) {
        (void)snprintf(response, response_len_max, "{\"code\":\"SUCCESS\", \"message\":\"%s\", \"response\":\"%.1f deg, %.1f hPa %.1f rH\"}\r\n", sensor_returnCodeAsString(result), data[0].temperature_in_deg, data[0].pressure_in_pascal / 100.0f, data[0].humidity_in_per100);
    }
    else {
        (void)snprintf(response, response_len_max, "{\"code\":\"FAILURE\", \"message\":\"%s\", \"response\":\"\"}\r\n", sensor_returnCodeAsString(result));
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

void command_execute(command_index_e command_index, const uint32_t response_len_max, char *response) {
    if((response == NULL) || (response_len_max <= 0)) {
        return;
    }

    if(command_index < COMMAND_INDEX_MAX) {
        command_list[command_index].handler(response, response_len_max);
    }
}
