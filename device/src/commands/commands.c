/**
 * @file
 */

#include "commands.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "protocol.h"
#include "sensor.h"

#ifndef TEST
#include "FreeRTOS.h"
#include "task.h"
#define STATIC static
#else
#include "mock_freertos.h"
#define STATIC
#endif  // TEST

#define COMMAND_STRING_LEN_MAX (255u)

typedef struct {
    const char *name;
    void (*handler)(char *response, const uint32_t response_len_max);
    const char *desc;
} command_s;

STATIC void command_unknown(char *response, const uint32_t response_len_max);
STATIC void command_help(char *response, const uint32_t response_len_max);
STATIC void command_reboot(char *response, const uint32_t response_len_max);
STATIC void command_sensorSelfTest(char *response,
                                   const uint32_t response_len_max);
STATIC void command_sensorGetData(char *response,
                                  const uint32_t response_len_max);
STATIC void command_systemGetInfos(char *response,
                                   const uint32_t response_len_max);

STATIC const command_s command_list[] = {
    [COMMAND_UNKNOWN] = {"", command_unknown, ""},
    [COMMAND_HELP] = {"help", command_help, "Display all commands available"},
    [COMMAND_REBOOT] = {"reboot", command_reboot, "Performing a system reboot"},
    [COMMAND_SENSOR_SELF_TEST] = {"sensor_selfTest", command_sensorSelfTest,
                                  "Performing a sensor self-test"},
    [COMMAND_SENSOR_GET_DATA] = {"sensor_getData", command_sensorGetData,
                                 "Request a sensor to get data"},
    [COMMAND_SYSTEM_GET_INFOS] = {"system_getInfos", command_systemGetInfos,
                                  "Get system informations"},
};

STATIC void command_unknown(char *response, const uint32_t response_len_max) {
    (void)snprintf(
        response, response_len_max,
        "Command unknown, tap \"help\" to display all available command.\r\n");
}

STATIC void command_help(char *response, const uint32_t response_len_max) {
    uint32_t response_size =
        snprintf(response, response_len_max,
                 "All available command with description below :\r\n");
    response += response_size;
    for (uint32_t i = COMMAND_HELP + 1;
         i < sizeof(command_list) / sizeof(*command_list); i++) {
        response_size =
            snprintf(response, response_len_max, "  %s\r\t\t\t%s\r\n",
                     command_list[i].name, command_list[i].desc);
        response += response_size;
    }
}

STATIC void command_reboot(char *response, const uint32_t response_len_max) {
    (void)snprintf(response, response_len_max, "%s",
                   command_list[COMMAND_REBOOT].desc);
}

STATIC void command_sensorSelfTest(char *response,
                                   const uint32_t response_len_max) {
    int8_t result = sensor_selfTest();
    protocol_s protocol = {0};
    protocol.request.code = (result == SENSOR_OK)
                                ? PROTOCOL_REQUEST_CODE_SUCCESS
                                : PROTOCOL_REQUEST_CODE_FAILURE,
    (void)snprintf(protocol.request.message, response_len_max, "%s",
                   sensor_returnCodeAsString(result));

    protocol_serialize(protocol, response_len_max, response);
}

STATIC void command_sensorGetData(char *response,
                                  const uint32_t response_len_max) {
    sensor_data_s sensor_data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t sensor_data_number = 0;
    int8_t result = sensor_getData(sensor_data, &sensor_data_number);
    protocol_s protocol = {0};
    /* Get only the first data from sensor */
    if (result == SENSOR_OK) {
        protocol.data_nb = 3;
        protocol_data_s data[] = {
            [0] = {.name = "temperature",
                   .value = sensor_data[0].temperature_in_deg,
                   .unit = "degC"},
            [1] = {.name = "humidity",
                   .value = sensor_data[0].humidity_in_per100,
                   .unit = "per100"},
            [2] = {.name = "pressure",
                   .value = sensor_data[0].pressure_in_pascal,
                   .unit = "Pa"}};
        (void)memcpy(protocol.data, data, sizeof(data));
    }
    protocol.request.code = (result == SENSOR_OK)
                                ? PROTOCOL_REQUEST_CODE_SUCCESS
                                : PROTOCOL_REQUEST_CODE_FAILURE;
    (void)snprintf(protocol.request.message, response_len_max, "%s",
                   sensor_returnCodeAsString(result));

    protocol_serialize(protocol, response_len_max, response);
}

STATIC void command_systemGetInfos(char *response,
                                   const uint32_t response_len_max) {
    (void)response_len_max;
    vTaskGetRunTimeStats(response);
}

command_index_e command_getIndex(const char *cmd) {
    if (cmd == NULL) {
        return COMMAND_UNKNOWN;
    }
    for (uint32_t index = 0; index < sizeof(command_list) / sizeof(command_s);
         index++) {
        uint32_t cmp =
            strncmp(cmd, command_list[index].name, COMMAND_STRING_LEN_MAX);
        if (cmp == 0) {
            return index;
        }
    }
    return COMMAND_UNKNOWN;
}

void command_execute(command_index_e command_index,
                     const uint32_t response_len_max, char *response) {
    if ((response == NULL) || (response_len_max <= 0)) {
        return;
    }

    if (command_index < COMMAND_INDEX_MAX) {
        command_list[command_index].handler(response, response_len_max);
    }
}
