#include "commands.h"

#include "sensor.h"
#include "protocol.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#ifndef TEST
#define STATIC static
#else
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
    protocol_s data = {
        .code = PROTOCOL_CODE_FAILURE,
        .message = "Command unknown",
        .response = "Tap help to display all available command.",
    };
    protocol_serialize(data, response_len_max, response);
}

STATIC void command_help(char *response, const uint32_t response_len_max) {
    protocol_s data = {
        .code = PROTOCOL_CODE_SUCCESS,
        .message = "",
        .response = "",
    };
    (void)snprintf(data.response, response_len_max, "%s: %s, %s: %s, %s: %s", command_list[COMMAND_REBOOT].name, command_list[COMMAND_REBOOT].desc,command_list[COMMAND_SENSOR_SELF_TEST].name, command_list[COMMAND_SENSOR_SELF_TEST].desc, command_list[COMMAND_SENSOR_GET_DATA].name, command_list[COMMAND_SENSOR_GET_DATA].desc);
    protocol_serialize(data, response_len_max, response);
}

STATIC void command_reboot(char *response, const uint32_t response_len_max) {
    protocol_s data = {
        .code = PROTOCOL_CODE_SUCCESS,
        .message = "",
        .response = "",
    };
    protocol_serialize(data, response_len_max, response);
}

STATIC void command_sensorSelfTest(char *response, const uint32_t response_len_max) {
    protocol_s data = {0};
    int8_t result = sensor_selfTest();
    data.code = (result == SENSOR_OK) ? PROTOCOL_CODE_SUCCESS : PROTOCOL_CODE_FAILURE;
    (void)snprintf(data.message, response_len_max, "%s", sensor_returnCodeAsString(result));
    protocol_serialize(data, response_len_max, response);
}

STATIC void command_sensorGetData(char *response, const uint32_t response_len_max) {
    protocol_s protocol_data = {0};
    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t result = sensor_getData(data, &number_of_data);
    if(result == SENSOR_OK) {
        (void)snprintf(protocol_data.response, response_len_max, "%.1f deg, %.1f hPa %.1f rH", data[0].temperature_in_deg, data[0].pressure_in_pascal / 100.0f, data[0].humidity_in_per100);
    }
    protocol_data.code = (result == SENSOR_OK) ? PROTOCOL_CODE_SUCCESS : PROTOCOL_CODE_FAILURE;
    (void)snprintf(protocol_data.message, response_len_max, "%s", sensor_returnCodeAsString(result));
    protocol_serialize(protocol_data, response_len_max, response);
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
